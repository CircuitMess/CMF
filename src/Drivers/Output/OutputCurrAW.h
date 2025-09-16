#ifndef CMF_OUTPUTCURRAW_H
#define CMF_OUTPUTCURRAW_H

#include "Drivers/Interface/OutputDriver.h"
#include "Devices/AW9523.h"

class OutputCurrAW : public OutputDriver {
	GENERATED_BODY(OutputCurrAW, OutputDriver);

public:
	OutputCurrAW() = default;
	OutputCurrAW(const std::vector<OutputPinDef>& outputs, AW9523* aw9523);

private:
	StrongObjectPtr<AW9523> aw9523;

	void performWrite(int port, float value) noexcept override;
	void performRegister(const OutputPinDef& output) noexcept override;
};


#endif //CMF_OUTPUTCURRAW_H
