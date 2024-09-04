#include "ButtonInput.h"
#include "Log/Log.h"

void ButtonInput::reg(Enum<int> button, InputPin pin){
	inputSources.insert(StrongObjectPtr<InputDriver>(pin.driver));
	buttons[button] = pin;
}

bool ButtonInput::getState(Enum<int> button){
	if(!buttons.contains(button)){
		CMF_LOG(LogCMF, Error, "Button %d not registered", (int) button);
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
