#include "InputDriver.h"
#include "Log/Log.h"

InputDriver::InputDriver(const std::vector<InputPinDef>& inputs) : inputs(inputs){

}

void InputDriver::scan() noexcept{

}

float InputDriver::read(int port) const noexcept{
	if(!states.contains(port)){
		CMF_LOG(LogCMF, Error, "Input port %d not registered", port);
		return 0;
	}
	return states.at(port);
}

void InputDriver::registerInput(InputPinDef pinDef){
	inputs.emplace_back(pinDef);
	performRegister(pinDef);
}

void InputDriver::removeInput(int port){
	auto it = std::remove_if(inputs.begin(), inputs.end(), [port](const InputPinDef& pinDef){
		return pinDef.port == port;
	});
	for(auto i = it; i != inputs.end(); ++i){
		performDeregister(*i);
		states.erase(i->port);
	}
	inputs.erase(it, inputs.end());
}

void InputDriver::removeInput(InputPinDef input){
	auto it = std::remove(inputs.begin(), inputs.end(), input);
	for(auto i = it; i != inputs.end(); ++i){
		performDeregister(*i);
		states.erase(i->port);
	}
	inputs.erase(it, inputs.end());
}

void InputDriver::begin() noexcept{
	Super::begin();

	scan();
}

void InputDriver::performRegister(InputPinDef input){

}

void InputDriver::performDeregister(InputPinDef input){

}

void InputDriver::postInitProperties() noexcept{
	SyncEntity::postInitProperties();

	for(const auto& input: inputs){
		performRegister(input);
	}
}

std::vector<InputPinDef>& InputDriver::getInputs(){
	return inputs;
}

std::map<int, bool>& InputDriver::getStates(){
	return states;
}

void InputDriver::forEachInput(std::function<void(const InputPinDef&)> func) const{
	for(const auto& input: inputs){
		func(input);
	}
}
