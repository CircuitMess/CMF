#include "RM_IRModule.h"

RM_IRModule::RM_IRModule(const Modules::BusPins& busPins) : ModuleDevice(Type::RM_IR, busPins){
	write(false);
}

void RM_IRModule::write(bool state){
	if(pins.subAddressPins[1].outputDriver){
		pins.subAddressPins[1].outputDriver->write(pins.subAddressPins[1].port, state);
	}
}

bool RM_IRModule::read(){
	if(pins.subAddressPins[0].inputDriver){
		return pins.subAddressPins[0].inputDriver->read(pins.subAddressPins[0].port);
	}
	return false;
}