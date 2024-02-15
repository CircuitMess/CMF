#include "Class.h"
#include "Object.h"

const class Class* ClassRegistry::getClass(uint32_t ID) const noexcept{
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

uint32_t Class::getID() const noexcept{
	return classID;
}

Class::Class(uint32_t ID) noexcept : classID(ID) {
	if(registry == nullptr){
		registry = new ClassRegistry();
	}

	registry->registerClass(this);
}

Object* Class::createDefaultObject()  const noexcept {
	void* temp = operator new(sizeof(Object));
	memset(temp, 0, sizeof(Object));
	return new(temp) Object();
}
