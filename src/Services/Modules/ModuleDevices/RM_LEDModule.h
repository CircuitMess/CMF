#ifndef CMF_RM_LEDMODULE_H
#define CMF_RM_LEDMODULE_H

#include "Services/Modules/ModuleDevice.h"

class RM_LEDModule : public ModuleDevice {
	GENERATED_BODY(RM_LEDModule, ModuleDevice)
public:
	RM_LEDModule(const Modules::BusPins& busPins = {});

	void set(bool state);
};


#endif //CMF_RM_LEDMODULE_H
