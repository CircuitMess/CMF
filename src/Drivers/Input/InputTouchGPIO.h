#ifndef CMF_INPUTTOUCHGPIO_H
#define CMF_INPUTTOUCHGPIO_H

#include "Drivers/Interface/InputDriver.h"

struct TouchPinDef : InputPinDef {
	uint32_t threshold;
};

/* TODO - refactor using new ESP-IDF Touch API */

class InputTouchGPIO : public InputDriver {
	GENERATED_BODY(InputTouchGPIO, InputDriver);

public:
	InputTouchGPIO() noexcept = default;
	InputTouchGPIO(const std::vector<TouchPinDef>& inputs) noexcept;

	void registerInput(const TouchPinDef& pinDef);

private:
	void scan() noexcept override;

	void performRegister(InputPinDef input) noexcept override;

//	std::map<int, uint32_t> benchmarks;
	std::map<int, uint32_t> thresholds;
};

#endif //CMF_INPUTTOUCHGPIO_H
