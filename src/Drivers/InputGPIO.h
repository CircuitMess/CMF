#ifndef CMF_INPUTGPIO_H
#define CMF_INPUTGPIO_H

#include "Drivers/Interface/InputDriver.h"
#include "Periphery/GPIO.h"

class InputGPIO : public InputDriver {
	GENERATED_BODY(InputGPIO, InputDriver);

public:
	InputGPIO() = default;
	InputGPIO(const std::vector<InputPinDef>& inputs, StrongObjectPtr<GPIO> gpio);
	void begin() noexcept override;

private:
	void scan() noexcept override;

	StrongObjectPtr<GPIO> gpio;
};


#endif //CMF_INPUTGPIO_H
