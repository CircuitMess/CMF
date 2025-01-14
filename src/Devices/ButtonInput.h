#ifndef CMF_TEMPLATE_BUTTONINPUT_H
#define CMF_TEMPLATE_BUTTONINPUT_H

#include "Drivers/Interface/InputDriver.h"
#include "Entity/AsyncEntity.h"

class ButtonInput : public AsyncEntity {
	GENERATED_BODY(ButtonInput, AsyncEntity);

public:
	void reg(Enum<int> button, InputPin pin) noexcept;

	bool getState(Enum<int> button) noexcept;

protected:
	void tick(float deltaTime) noexcept override;

private:
	std::set<StrongObjectPtr<InputDriverBase>> inputSources;

	std::map<int, InputPin> buttons;
};

#endif //CMF_TEMPLATE_BUTTONINPUT_H
