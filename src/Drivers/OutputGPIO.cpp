#include "OutputGPIO.h"
#include "Log/Log.h"

OutputGPIO::OutputGPIO(const std::vector<OutputPinDef>& outputs, StrongObjectPtr<GPIO> gpio) : OutputDriver(outputs), gpio(std::move(gpio)){

}

void OutputGPIO::write(int port, bool value) noexcept{
	Super::write(port, value);

	const auto pin = (gpio_num_t) port;
	if(!getInversions().contains(port)){
		CMF_LOG(LogCMF, Warning, "Output port %d inversion missing", port);
		return;
	}
	gpio->write(pin, value ^ getInversions()[port]);
}

void OutputGPIO::performRegister(OutputPinDef output){
	const auto pin = (gpio_num_t) output.port;
	gpio->setMode(pin, GPIOMode::Output);
	//setting initial state
	getInversions()[output.port] = output.inverted;
	write((gpio_num_t)output.port, false);
}
