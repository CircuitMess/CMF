#include "OutputPWM.h"
#include "Log/Log.h"

DEFINE_LOG(PWM)

OutputPWM::OutputPWM(const std::vector<OutputPWMPinDef>& outputs) : OutputDriver(OutputDriver::toOutputPinDef(outputs)){
	for(const auto& output : outputs){
		gpios[output.port] = output.pin;
	}
}

void OutputPWM::setFreq(int port, uint16_t freq){
	if(!checkFrequency(freq)){
		CMF_LOG(PWM, Warning, "couldnt write frequency %d because of clock divisor limitations\n", freq);
		return;
	}

	const auto group = getSpeedMode(port);
	const auto timer = getTimer(port);
	const auto chan = getChannel(port);

	ledc_set_freq(group, timer, freq);
	ledc_update_duty(group, chan);
}

void OutputPWM::stop(int port){
	ledc_stop(getSpeedMode(port), getChannel(port), getInversions()[port]);
}

void OutputPWM::attach(int port){
	if(!gpios.count(port)) return;

	const ledc_channel_config_t ledc_channel = {
			.gpio_num       = gpios[port],
			.speed_mode     = getSpeedMode(port),
			.channel        = getChannel(port),
			.intr_type      = LEDC_INTR_DISABLE,
			.timer_sel      = getTimer(port),
			.duty           = 0,
			.hpoint         = 0,
			.flags = { .output_invert = false } //this is accounted for in parent abstraction
	};
	ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void OutputPWM::detach(int port){
	if(!gpios.count(port)) return;

	const auto pin = gpios[port];

	const gpio_config_t cfg = {
			.pin_bit_mask = 1ULL << pin,
			.mode = GPIO_MODE_OUTPUT
	};

	gpio_config(&cfg);
	gpio_set_level((gpio_num_t) pin, getInversions()[port]);
}

void OutputPWM::performWrite(int port, float value) noexcept{
	const uint32_t duty = (uint32_t) (FullDuty * value);
	const auto group = getSpeedMode(port);
	const auto chan = getChannel(port);

	ledc_set_duty(group, chan, duty);
	ledc_update_duty(group, chan);
}

void OutputPWM::performRegister(OutputPinDef output) noexcept{
	if(!gpios.contains(output.port)){
		ESP_LOGE("OutputPWM", "Output port %d not found in pin map", output.port);
		return;
	}


	const auto& pin = gpios[output.port];

	gpio_reset_pin(pin);
	gpio_set_direction(pin, GPIO_MODE_OUTPUT);

	const ledc_timer_config_t ledc_timer = {
			.speed_mode       = getSpeedMode(output.port),
			.duty_resolution  = DutyResDefault,
			.timer_num        = getTimer(output.port),
			.freq_hz          = DefaultFreq,
			.clk_cfg          = LEDC_AUTO_CLK
	};

	if(ledc_timer_config(&ledc_timer) != ESP_OK){
		CMF_LOG(PWM, Error, "timer config failed!");
		return;
	}

	attach(output.port);
}

void OutputPWM::performDeregister(OutputPinDef output) noexcept{
	stop(output.port);
	detach(output.port);
}
