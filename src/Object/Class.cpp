#include "Class.h"
#include "Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"

const Class* ClassRegistry::getClass(uint64_t ID) const noexcept{
	if(!classes.contains(ID)){
		return nullptr;
	}

	return classes.at(ID);
}

void ClassRegistry::registerClass(const Class* cls) noexcept{
	if(classes.contains(cls->getID())){
		return;
	}

	classes[cls->getID()] = cls;
}

StrongObjectPtr<Object> Class::__createObject(void* arguments) const noexcept {
	void* temp = operator new(sizeof(Object));
	if(temp == nullptr){
		return nullptr;
	}

	memset(temp, 0, sizeof(Object));

	// This is used to make sure the new object is valid in its constructor
	StrongObjectPtr<Object> tempPtr = static_cast<Object*>(temp);

	return new(temp) Object();
}
