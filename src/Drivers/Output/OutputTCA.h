#ifndef CMF_OUTPUTTCA_H
#define CMF_OUTPUTTCA_H

#include "Drivers/Interface/OutputDriver.h"
#include "Devices/TCA9555.h"

class OutputTCA : public OutputDriver {
	GENERATED_BODY(OutputTCA, OutputDriver);

public:
	OutputTCA() = default;
	OutputTCA(const std::vector<OutputPinDef>& outputs, TCA9555* tca9555);

	void performWrite(int port, float value) noexcept override;
	void performRegister(const OutputPinDef& output) noexcept override;

private:
	StrongObjectPtr<TCA9555> tca9555;
};


#endif //CMF_OUTPUTTCA_H
