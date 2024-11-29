#ifndef CMF_OUTPUTDIGAW_H
#define CMF_OUTPUTDIGAW_H

#include "Drivers/Interface/OutputDriver.h"
#include "Devices/AW9523.h"

class OutputDigAW : public OutputDriver<> {
	GENERATED_BODY(OutputDigAW, OutputDriver);

public:
	OutputDigAW() = default;
	OutputDigAW(const std::vector<OutputPinDef>& outputs, AW9523* aw9523);

private:
	void performWrite(int port, float value) noexcept override;
	void performRegister(OutputPinDef output) noexcept override;

	StrongObjectPtr<AW9523> aw9523;
};


#endif //CMF_OUTPUTDIGAW_H
