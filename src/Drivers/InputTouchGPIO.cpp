#include "InputTouchGPIO.h"
#include <driver/touch_pad.h>

InputTouchGPIO::InputTouchGPIO(const std::vector<TouchPinDef>& inputs) : InputDriver(inputs){
	touch_pad_init();

	touch_pad_denoise_t denoise = {
			.grade = TOUCH_PAD_DENOISE_BIT4,
			.cap_level = TOUCH_PAD_DENOISE_CAP_L4,
	};
	touch_pad_denoise_set_config(&denoise);
	touch_pad_denoise_enable();

	touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
	touch_pad_fsm_start();
}

void InputTouchGPIO::scan() noexcept{
	const auto touchStatus = touch_pad_get_status();

	forEachInput([this, &touchStatus](const TouchPinDef& input){
		getStates()[input.port] = (touchStatus & (1UL << input.port)) ^ input.inverted;
	});
}

void InputTouchGPIO::performRegister(TouchPinDef input){
	touch_pad_fsm_stop();

	const auto touchPin = (touch_pad_t) input.port;
	touch_pad_config(touchPin);
	touch_pad_set_thresh(touchPin, input.threshold);

	touch_pad_fsm_start();

	/*vTaskDelay(100 / portTICK_PERIOD_MS);

	uint32_t benchmark;
	ESP_ERROR_CHECK(touch_pad_read_benchmark(touchPin, &benchmark));
	printf("%d benchmark: %lu\n", input.port, benchmark);
	benchmarks[input.port] = benchmark;*/
}
