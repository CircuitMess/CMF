#include "ButtonInput.h"
#include "Log/Log.h"

void ButtonInput::reg(Enum<int> button, InputPin pin) noexcept{
	inputSources.insert(StrongObjectPtr<InputDriverBase>(pin.driver));
	buttons[button] = pin;
}

bool ButtonInput::getState(Enum<int> button) noexcept{
	if(!buttons.contains(button)){
		CMF_LOG(CMF, Error, "Button %d not registered", (int) button);
		return false;
	}
	const auto& inputPin = buttons[button];
	bool res = (bool) inputPin.driver->read(inputPin.port);

	return res;
}

void ButtonInput::tick(float deltaTime) noexcept{
	for(auto& source: inputSources){
		source->scan();
	}
}
