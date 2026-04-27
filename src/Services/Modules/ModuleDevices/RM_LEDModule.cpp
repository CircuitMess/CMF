#include "RM_LEDModule.h"

RM_LEDModule::RM_LEDModule(const Modules::BusPins& busPins) : Super(Modules::Type::RM_LED, busPins){
	set(false);
}

void RM_LEDModule::set(bool state){
	setLevel(state ? 1.0f : 0.0f);
}

void RM_LEDModule::setLevel(float level){
	ESP_LOGI("RM_LEDModule", "Setting LED level to %.2f", level);
	if(pins.subAddressPins[0].outputDriver){
		pins.subAddressPins[0].outputDriver->write(pins.subAddressPins[0].port, level);
	}
}
