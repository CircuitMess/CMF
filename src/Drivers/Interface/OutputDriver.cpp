#include "OutputDriver.h"
#include "Log/Log.h"

OutputDriver::OutputDriver(const std::vector<OutputPinDef>& outputs) : outputs(outputs){

}

float OutputDriver::getState(int port) noexcept{
	if(!states.contains(port)){
		CMF_LOG(LogCMF, Error, "Port %d not registered", port);
	}
	return states[port];
}

void OutputDriver::write(int port, float value) noexcept{
	states[port] = value;

}

void OutputDriver::send() noexcept{

}

void OutputDriver::write(int port, bool value) noexcept{
	write(port, value ? 1.f : 0.f);
}
