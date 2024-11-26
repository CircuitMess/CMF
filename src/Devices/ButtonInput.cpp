#include "ButtonInput.h"
#include "Log/Log.h"
#include "Memory/Cast.h"

void ButtonInput::reg(Enum<int> button, InputPin pin){
	inputSources.insert(StrongObjectPtr<InputDriver<InputPinDef>>(pin.driver));
	buttons[button] = pin;
}

bool ButtonInput::getState(Enum<int> button){
	if(!buttons.contains(button)){
		CMF_LOG(CMF, Error, "Button %d not registered", (int) button);
		return false;
	}
	const auto& inputPin = buttons[button];
	auto* inputDriver = cast<InputDriver<InputPinDef>>(inputPin.driver);
	bool res = (bool) inputDriver->read(inputPin.port);

	return res;
}

void ButtonInput::tick(float deltaTime) noexcept{
	for(auto& source: inputSources){
		source->scan();
	}
}
