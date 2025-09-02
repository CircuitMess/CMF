#include "ModuleDevice.h"

ModuleDevice::ModuleDevice(ModuleType moduleType) : Object(), moduleType(moduleType){

}

ModuleType ModuleDevice::getType() const{
	return moduleType;
}

void ModuleDevice::setI2C(I2C* i2c){
	this->i2c = i2c;
}

void ModuleDevice::setInputs(std::array<InputPin, 6> inputPins){
	this->inputPins = inputPins;
}

void ModuleDevice::setOutputs(std::array<OutputPin, 6> outputPins){
	this->outputPins = outputPins;
}