#include "InputDriver.h"
#include "Log/Log.h"

InputDriver::InputDriver(const std::vector<InputPinDef>& inputs) : inputs(inputs){

}

void InputDriver::scan() noexcept{

}

float InputDriver::read(int port) noexcept{
	if(!states.contains(port)){
		CMF_LOG(LogCMF, Error, "Port %d not registered", port);
	}
	return states[port];
}
