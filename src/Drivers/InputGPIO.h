#ifndef CMF_INPUTGPIO_H
#define CMF_INPUTGPIO_H

#include "Drivers/Interface/InputDriver.h"
#include "Periphery/GPIO.h"

enum class PullMode : uint8_t {
	None, Up, Down
};

struct GPIOPinDef : InputPinDef {
	PullMode pullMode;
};

class InputGPIO : public InputDriver<GPIOPinDef> {
	GENERATED_BODY(InputGPIO, InputDriver);

public:
	InputGPIO() = default;
	InputGPIO(const std::vector<GPIOPinDef>& inputs, StrongObjectPtr<GPIO> gpio);

private:
	void scan() noexcept override;

	StrongObjectPtr<GPIO> gpio;

	void performRegister(GPIOPinDef input) override;
};

#endif //CMF_INPUTGPIO_H
