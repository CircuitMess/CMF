#ifndef CMF_OUTPUTGPIO_H
#define CMF_OUTPUTGPIO_H

#include "Drivers/Interface/OutputDriver.h"
#include "Periphery/GPIO.h"

class OutputGPIO : public OutputDriver {
	GENERATED_BODY(OutputGPIO, OutputDriver);
public:
	OutputGPIO() = default;
	OutputGPIO(const std::vector<OutputPinDef>& outputs, StrongObjectPtr<GPIO> gpio);

	void write(int port, bool value) noexcept override;
	void begin() noexcept override;

private:
	StrongObjectPtr<GPIO> gpio;
};


#endif //CMF_OUTPUTGPIO_H
