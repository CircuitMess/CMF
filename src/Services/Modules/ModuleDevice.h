#ifndef CMF_MODULEDEVICE_H
#define CMF_MODULEDEVICE_H

#include "Object/Object.h"
#include "ModuleType.h"
#include "Periphery/I2C.h"
#include "Drivers/Interface/InputDriver.h"
#include "Drivers/Interface/OutputDriver.h"

class ModuleDevice : public Object {
	GENERATED_BODY(ModuleDevice, Object)
public:
	ModuleDevice(ModuleType moduleType = ModuleType::Unknown);

	ModuleType getType() const;

	void setI2C(I2C* i2c);
	void setInputs(std::array<InputPin, 6> inputPins);
	void setOutputs(std::array<OutputPin, 6> outputPins);

private:
	const ModuleType moduleType;

	I2C* i2c;
	std::array<InputPin, 6> inputPins;
	std::array<OutputPin, 6> outputPins;
};


#endif //CMF_MODULEDEVICE_H
