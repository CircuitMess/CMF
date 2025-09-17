#ifndef CMF_RM_IRMODULE_H
#define CMF_RM_IRMODULE_H

#include "Services/Modules/ModuleDevice.h"

class RM_IRModule : public ModuleDevice {
	GENERATED_BODY(RM_IRModule, ModuleDevice)
public:
	RM_IRModule(const Modules::BusPins& busPins = {});

	void write(bool state);
	bool read();
};


#endif //CMF_RM_IRMODULE_H
