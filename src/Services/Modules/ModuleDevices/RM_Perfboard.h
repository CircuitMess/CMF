#ifndef CMF_RM_PERFBOARD_H
#define CMF_RM_PERFBOARD_H

#include "Services/Modules/ModuleDevice.h"

class RM_Perfboard : public ModuleDevice{
	GENERATED_BODY(RM_Perfboard, ModuleDevice)
public:
	RM_Perfboard(const Modules::BusPins& busPins = {});
};


#endif //CMF_RM_PERFBOARD_H
