#ifndef CMF_INPUTTCA_H
#define CMF_INPUTTCA_H

#include "Drivers/Interface/InputDriver.h"
#include "Devices/TCA9555.h"

class InputTCA : public InputDriver<> {
	GENERATED_BODY(InputTCA, InputDriver)
public:
	InputTCA() = default;
	InputTCA(const std::vector<InputPinDef>& inputs, TCA9555* tca9555);

private:
	void scan() noexcept override;

	void performRegister(InputPinDef input) noexcept override;

	StrongObjectPtr<TCA9555> tca9555;
};


#endif //CMF_INPUTTCA_H
