#ifndef CMF_MODULEDEVICE_H
#define CMF_MODULEDEVICE_H

#include "Object/Object.h"
#include "ModuleType.h"
#include "Periphery/I2C.h"
#include "Drivers/Interface/InputDriver.h"
#include "Drivers/Interface/OutputDriver.h"

using namespace Modules;

class ModuleDevice : public Object {
	GENERATED_BODY(ModuleDevice, Object)
public:
	ModuleDevice(Type Type = Type::Unknown, const Modules::BusPins& busPins={});

	Type getType() const;

protected:
	const Type moduleType;

	Modules::BusPins pins;
};


#endif //CMF_MODULEDEVICE_H
