#include "RM_IRModule.h"
#include <driver/rmt_tx.h>
#include <driver/rmt_rx.h>
#include <driver/rmt_encoder.h>
#include <driver/gpio.h>
#include <esp_log.h>
#include <Util/stdafx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstring>

static constexpr const char* TAG = "RM_IRModule";

static constexpr uint32_t RMTResolutionHz = 1000000; // 1 MHz = 1 µs per tick
static constexpr uint32_t IRCarrierHz = 38000;
static constexpr uint32_t EndOfFrameMinNs = 10000000; // 10 ms gap = end-of-frame

RM_IRModule::RM_IRModule(const Modules::BusPins& busPins) : Super(Modules::Type::RM_IR, busPins){
	write(false);
}

void RM_IRModule::write(bool state){
	if(pins.subAddressPins[1].outputDriver){
		pins.subAddressPins[1].outputDriver->write(pins.subAddressPins[1].outputPort, state);
	}
}

bool RM_IRModule::read(){
	if(pins.subAddressPins[0].inputDriver){
		return pins.subAddressPins[0].inputDriver->read(pins.subAddressPins[0].outputPort);
	}

	return false;
}

gpio_num_t RM_IRModule::getInputGPIO() const{
	return static_cast<gpio_num_t>(pins.subAddressPins[0].inputPort);
}

gpio_num_t RM_IRModule::getOutputGPIO() const{
	return static_cast<gpio_num_t>(pins.subAddressPins[1].outputPort);
}

IRSignal RM_IRModule::captureSignal(uint32_t timeoutMs){
	IRSignal result{};

	const gpio_num_t rxPin = getInputGPIO();

	// Fully reset the pin so LEDC/PWM output is disconnected before RMT takes over.
	gpio_reset_pin(rxPin);

	rmt_rx_channel_config_t rxConfig{};
	rxConfig.gpio_num = rxPin;
	rxConfig.clk_src = RMT_CLK_SRC_DEFAULT;
	rxConfig.resolution_hz = RMTResolutionHz;
	rxConfig.mem_block_symbols = IRMaxSymbols;

	rmt_channel_handle_t rxChannel = nullptr;
	if(rmt_new_rx_channel(&rxConfig, &rxChannel) != ESP_OK){
		ESP_LOGE(TAG, "Failed to create RMT RX channel");
		return result;
	}

	rmt_symbol_word_t rawBuf[IRMaxSymbols];
	StaticQueue_t queueControl;
	uint8_t queueStorage[sizeof(rmt_rx_done_event_data_t)];
	QueueHandle_t queue = xQueueCreateStatic(1, sizeof(rmt_rx_done_event_data_t), queueStorage, &queueControl);

	rmt_rx_event_callbacks_t cbs{};
	cbs.on_recv_done = [](rmt_channel_handle_t, const rmt_rx_done_event_data_t* data, void* ctx) -> bool {
		BaseType_t woken = pdFALSE;
		xQueueSendFromISR(static_cast<QueueHandle_t>(ctx), data, &woken);
		return woken == pdTRUE;
	};
	rmt_rx_register_event_callbacks(rxChannel, &cbs, queue);
	rmt_enable(rxChannel);

	rmt_receive_config_t receiveConfig{};
	receiveConfig.signal_range_min_ns = 1250;
	receiveConfig.signal_range_max_ns = EndOfFrameMinNs;

	if(rmt_receive(rxChannel, rawBuf, IRMaxSymbols * sizeof(rmt_symbol_word_t), &receiveConfig) != ESP_OK){
		ESP_LOGE(TAG, "rmt_receive failed");
	} else {
		rmt_rx_done_event_data_t rxData{};
		if(xQueueReceive(queue, &rxData, pdMS_TO_TICKS(timeoutMs)) == pdTRUE){
			const uint16_t count = static_cast<uint16_t>(rxData.num_symbols);
			if(count > 0){
				const uint16_t stored = (count <= IRMaxSymbols) ? count : IRMaxSymbols;
				memcpy(result.symbols, rxData.received_symbols, stored * sizeof(rmt_symbol_word_t));
				result.count = stored;
			}
		}
	}

	rmt_disable(rxChannel);
	rmt_del_channel(rxChannel);
	vQueueDelete(queue);

	// Reset the pin fully before reconnecting LEDC so it sees a clean state.
	gpio_reset_pin(rxPin);
	if(pins.subAddressPins[0].outputDriver){
		pins.subAddressPins[0].outputDriver->registerOutput({ pins.subAddressPins[0].outputPort });
	}

	return result;
}

void RM_IRModule::replaySignal(const rmt_symbol_word_t* symbols, uint16_t count, uint32_t minDurationMs){
	if(!symbols || count == 0){
		return;
	}

	const gpio_num_t txPin = getOutputGPIO();

	rmt_tx_channel_config_t txConfig{};
	txConfig.gpio_num = txPin;
	txConfig.clk_src = RMT_CLK_SRC_DEFAULT;
	txConfig.resolution_hz = RMTResolutionHz;
	txConfig.mem_block_symbols = RMTMemBlockSymbols;
	txConfig.trans_queue_depth = 4;

	rmt_channel_handle_t txChannel = nullptr;
	if(rmt_new_tx_channel(&txConfig, &txChannel) != ESP_OK){
		ESP_LOGE(TAG, "Failed to create RMT TX channel");
		return;
	}

	rmt_carrier_config_t carrier{};
	carrier.frequency_hz = IRCarrierHz;
	carrier.duty_cycle = 0.33f;
	rmt_apply_carrier(txChannel, &carrier);

	rmt_copy_encoder_config_t copyConfig{};
	rmt_encoder_handle_t encoder = nullptr;
	if(rmt_new_copy_encoder(&copyConfig, &encoder) != ESP_OK){
		ESP_LOGE(TAG, "Failed to create RMT copy encoder");
		rmt_del_channel(txChannel);
		return;
	}

	rmt_enable(txChannel);

	uint32_t frameDurationUs = 0;
	for(uint16_t i = 0; i < count; ++i){
		frameDurationUs += symbols[i].duration0 + symbols[i].duration1;
	}
	const uint32_t minRepeatDurationUs = (minDurationMs * 1000 > frameDurationUs * 2)
		? minDurationMs * 1000
		: frameDurationUs * 2;

	uint32_t elapsedUs = 0;
	rmt_transmit_config_t txCfg{};
	txCfg.loop_count = 0;

	while(elapsedUs < minRepeatDurationUs){
		rmt_transmit(txChannel, encoder, symbols, count * sizeof(rmt_symbol_word_t), &txCfg);
		rmt_tx_wait_all_done(txChannel, pdMS_TO_TICKS(500));
		elapsedUs += frameDurationUs;
		delayMillis(40);
		elapsedUs += 40000;
	}

	rmt_disable(txChannel);
	rmt_del_encoder(encoder);
	rmt_del_channel(txChannel);
}