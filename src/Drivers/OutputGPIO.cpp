#include "OutputGPIO.h"
#include "Log/Log.h"

OutputGPIO::OutputGPIO(const std::vector<OutputPinDef>& outputs, StrongObjectPtr<GPIO> gpio) : OutputDriver(outputs), gpio(std::move(gpio)){
	for(const auto& output: this->outputs){
		const auto pin = (gpio_num_t) output.port;
		gpio->setMode(pin, GPIOMode::Output);

	}
}

void OutputGPIO::write(int port, bool value) noexcept{
	Super::write(port, value);

	const auto pin = (gpio_num_t) port;
	gpio->write(pin, value ^ inversions[port]);
}

void OutputGPIO::begin() noexcept{
	Super::begin();

	for(const auto& output: outputs){
		//setting initial state
		inversions[output.port] = output.inverted;
		write((gpio_num_t)output.port, false);
	}
}
