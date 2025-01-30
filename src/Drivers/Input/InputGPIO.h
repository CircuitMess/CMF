#ifndef CMF_INPUTGPIO_H
#define CMF_INPUTGPIO_H

#include "Drivers/Interface/InputDriver.h"
#include "Periphery/GPIOPeriph.h"

enum class PullMode : uint8_t {
	None, Up, Down
};

struct GPIOPinDef : InputPinDef {
	PullMode pullMode;
};

class InputGPIO : public InputDriver<GPIOPinDef> {
	GENERATED_BODY(InputGPIO, InputDriver);

public:
	InputGPIO() noexcept = default;
	InputGPIO(const std::vector<GPIOPinDef>& inputs, StrongObjectPtr<GPIOPeriph> gpio) noexcept;

private:
	void scan() noexcept override;

	void performRegister(GPIOPinDef input) noexcept override;

	StrongObjectPtr<GPIOPeriph> gpio;
};

#endif //CMF_INPUTGPIO_H
