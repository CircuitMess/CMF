#include "RM_LEDModule.h"

RM_LEDModule::RM_LEDModule(const Modules::BusPins& busPins) : ModuleDevice(Type::RM_LED, busPins){
	set(false);
}

void RM_LEDModule::set(bool state){
	ESP_LOGI("RM_LEDModule", "Setting LED to %d", state);
/*	if(pins.subAddressPins[0].outputDriver){
		ESP_LOGI("RM_LEDModule", "writing to driver");
		pins.subAddressPins[0].outputDriver->write(pins.subAddressPins[0].port, !state);
	}*/
	gpio_set_direction((gpio_num_t) pins.subAddressPins[0].port, GPIO_MODE_OUTPUT);
	gpio_set_level((gpio_num_t) pins.subAddressPins[0].port, !state);
}
