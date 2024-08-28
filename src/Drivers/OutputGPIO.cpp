#include "OutputGPIO.h"
#include "Log/Log.h"

OutputGPIO::OutputGPIO(const std::vector<OutputPinDef>& outputs, StrongObjectPtr<GPIO> gpio) : OutputDriver(outputs), gpio(std::move(gpio)){
	for(const auto& output: this->outputs){
		const auto pin = (gpio_num_t) output.port;
		gpio->setMode(pin, GPIOMode::Output);

	}
}

void OutputGPIO::write(int port, bool value) noexcept{
	if(states[port] == value){
		return;
	}
	const auto pin = (gpio_num_t) port;
	gpio->write(pin, value ^ inversions[port]);
	states[port] = value;
}

void OutputGPIO::begin() noexcept{
	Super::begin();

	for(const auto& output: outputs){

		//setting initial state
		gpio->write((gpio_num_t)output.port, output.inverted);

		states[output.port] = false;
		inversions[output.port] = output.inverted;
	}
}
