#ifndef CMF_OUTPUTGPIO_H
#define CMF_OUTPUTGPIO_H

#include "Drivers/Interface/OutputDriver.h"
#include "Periphery/GPIOPeriph.h"

class OutputGPIO : public OutputDriver {
	GENERATED_BODY(OutputGPIO, OutputDriver);
public:
	OutputGPIO() noexcept = default;
	OutputGPIO(const std::vector<OutputPinDef>& outputs, StrongObjectPtr<GPIOPeriph> gpio) noexcept;

private:
	void performRegister(OutputPinDef output) noexcept override;

	void performWrite(int port, float value) noexcept override;

	StrongObjectPtr<GPIOPeriph> gpio;
};

#endif //CMF_OUTPUTGPIO_H
