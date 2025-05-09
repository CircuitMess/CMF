#include "ButtonInput.h"
#include "Log/Log.h"

ButtonInput::ButtonInput() : Super(SleepTime / portTICK_PERIOD_MS, 4 * 1024, 4, -1){}

void ButtonInput::reg(const std::vector<std::pair<Enum<int>, InputPin>>& registrations) noexcept{
	for(auto& r: registrations){
		reg(r.first, r.second);
	}
}

void ButtonInput::reg(Enum<int> button, InputPin pin) noexcept{
	inputSources.insert(StrongObjectPtr<InputDriverBase>(pin.driver));
	buttons[button] = pin;
	dbTime[button] = 0;
	btnState[button] = false;
}

bool ButtonInput::getState(Enum<int> button) noexcept{
	if(!btnState.contains(button)){
		CMF_LOG(CMF, Error, "Button %d not registered", (int) button);
		return false;
	}
	return btnState[button];
}

void ButtonInput::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	for(auto& source: inputSources){
		source->scan();
	}

	for(const auto& pair: buttons){
		const auto btn = pair.first;
		const auto pin = pair.second;


		bool state = (bool) pin.driver->read(pin.port);
		if(state){
			pressed(btn);
		}else{
			released(btn);
		}
	}
}

void ButtonInput::pressed(int btn){
	if(btnState[btn]){
		dbTime[btn] = 0;
		return;
	}

	auto t = millis();

	if(dbTime[btn] == 0){
		dbTime[btn] = t;
		return;
	}else if(t - dbTime[btn] < DebounceTime){
		return;
	}

	btnState[btn] = true;
	dbTime[btn] = 0;

	OnButtonEvent.broadcast(btn, Action::Press);
}

void ButtonInput::released(int btn){
	if(!btnState[btn]){
		dbTime[btn] = 0;
		return;
	}

	auto t = millis();

	if(dbTime[btn] == 0){
		dbTime[btn] = t;
		return;
	}else if(t - dbTime[btn] < DebounceTime){
		return;
	}

	btnState[btn] = false;
	dbTime[btn] = 0;

	OnButtonEvent.broadcast(btn, Action::Release);
}
