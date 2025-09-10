#include "ModuleDevice.h"
#include "ModuleType.h"

ModuleDevice::ModuleDevice(Type moduleType, const Modules::BusPins& busPins) :
		Object(), moduleType(moduleType), pins(busPins){

}

Type ModuleDevice::getType() const{
	return moduleType;
}
