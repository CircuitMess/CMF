#include "RM_MotionSensor.h"

RM_MotionSensor::RM_MotionSensor(const Modules::BusPins& busPins) : Super(Modules::Type::RM_Motion, busPins){
	const auto pin = static_cast<gpio_num_t>(busPins.subAddressPins[0].port);
	const gpio_config_t io_conf = {
			.pin_bit_mask = 1ULL << pin,
			.mode = GPIO_MODE_INPUT,
			.pull_up_en = GPIO_PULLUP_DISABLE,
			.pull_down_en = GPIO_PULLDOWN_ENABLE,
			.intr_type = GPIO_INTR_ANYEDGE
	};

	gpio_intr_enable(pin);
	gpio_config(&io_conf);
	gpio_isr_handler_add(pin, isr, this);

	setLEDs(false);
}

void RM_MotionSensor::setLEDs(bool state){
	if(pins.subAddressPins[1].outputDriver){
		pins.subAddressPins[1].outputDriver->write(pins.subAddressPins[1].port, state);
	}
}

bool RM_MotionSensor::getState() const{
	return active;
}

void RM_MotionSensor::isr(void* arg){
	if(arg == nullptr){
		return;
	}

	RM_MotionSensor* motion = static_cast<RM_MotionSensor*>(arg);
	motion->active = gpio_get_level(static_cast<gpio_num_t>(motion->pins.subAddressPins[0].port));
}
