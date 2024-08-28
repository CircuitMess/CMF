#include "ButtonInput.h"

void ButtonInput::reg(Enum<int> button, InputPin pin){
	inputSources.insert(pin.driver);
	buttons[button] = pin;
}

bool ButtonInput::getState(Enum<int> button){
	const auto& inputPin = buttons[button];

	bool res = (bool) inputPin.driver->read(inputPin.port);

	return res;
}

void ButtonInput::tick(float deltaTime) noexcept{
	for(auto& source: inputSources){
		source->scan();
	}
}
