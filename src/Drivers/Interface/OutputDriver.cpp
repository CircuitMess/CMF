#include "OutputDriver.h"
#include "Log/Log.h"

OutputDriver::OutputDriver(const std::vector<OutputPinDef>& outputs) : outputs(outputs){

}

float OutputDriver::getState(int port) const noexcept{
	if(!states.contains(port)){
		CMF_LOG(LogCMF, Error, "Output port %d not registered", port);
		return 0;
	}
	return states.at(port);
}

void OutputDriver::write(int port, float value) noexcept{
	states[port] = value;

}

void OutputDriver::send() noexcept{

}

void OutputDriver::write(int port, bool value) noexcept{
	write(port, value ? 1.f : 0.f);
}

void OutputDriver::removeOutput(int port){
	auto it = std::remove_if(outputs.begin(), outputs.end(), [port](const OutputPinDef& pinDef){
		return pinDef.port == port;
	});
	for(auto i = it; i != outputs.end(); ++i){
		performDeregister(*i);
		states.erase(i->port);
		inversions.erase(i->port);
	}
	outputs.erase(it, outputs.end());
}

void OutputDriver::removeOutput(OutputPinDef output){
	auto it = std::remove(outputs.begin(), outputs.end(), output);
	for(auto i = it; i != outputs.end(); ++i){
		performDeregister(*i);
		states.erase(i->port);
		inversions.erase(i->port);
	}
	outputs.erase(it, outputs.end());
}

std::vector<OutputPinDef>& OutputDriver::getOutputs(){
	return outputs;
}

std::map<int, float>& OutputDriver::getStates(){
	return states;
}

std::map<int, bool>& OutputDriver::getInversions(){
	return inversions;
}

void OutputDriver::postInitProperties() noexcept{
	SyncEntity::postInitProperties();

	for(const auto& output: outputs){
		performRegister(output);
	}
}

void OutputDriver::performRegister(OutputPinDef output){

}

void OutputDriver::performDeregister(OutputPinDef output){

}
