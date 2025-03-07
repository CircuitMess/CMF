#ifndef CMF_INPUTTOUCHGPIO_H
#define CMF_INPUTTOUCHGPIO_H

#include "Drivers/Interface/InputDriver.h"
#include <hal/touch_sensor_types.h>

struct TouchPinDef : InputPinDef {
	uint32_t threshold;
};

class InputTouchGPIO : public InputDriver<TouchPinDef> {
	GENERATED_BODY(InputTouchGPIO, InputDriver);

public:
	InputTouchGPIO() noexcept = default;
	InputTouchGPIO(const std::vector<TouchPinDef>& inputs) noexcept;

private:
	void scan() noexcept override;

	void performRegister(TouchPinDef input) noexcept override;

//	std::map<int, uint32_t> benchmarks;
};

#endif //CMF_INPUTTOUCHGPIO_H
