#include "InputGPIO.h"
#include "Log/Log.h"

InputGPIO::InputGPIO(const std::vector<InputPinDef>& inputs, StrongObjectPtr<GPIO> gpio) : InputDriver(inputs), gpio(std::move(gpio)){
	for(const auto& input: this->inputs){
		const auto pin = (gpio_num_t) input.port;
		gpio->setMode(pin, GPIOMode::Input);
		switch(input.pullMode){
			case PullMode::Up:
				gpio->setPullup(pin, true);
				break;
			case PullMode::Down:
				gpio->setPulldown(pin, true);
				break;
			default:
				break;
		}
	}
}

void InputGPIO::scan() noexcept{
	Super::scan();
	for(const auto& input: inputs){
		const auto pin = (gpio_num_t) input.port;
		states[input.port] = gpio->read(pin) ^ input.inverted;
	}
}

void InputGPIO::begin() noexcept{
	Super::begin();

	scan();
}
