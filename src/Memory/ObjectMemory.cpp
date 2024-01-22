#include "ObjectMemory.h"
#include "Core/Application.h"

void initObject(Object* object, Object* owner) noexcept {
	if(object == nullptr){
		return;
	}

	if(owner != nullptr){
		object->setOwner(owner);
	}else{
		object->setOwner(ApplicationStatics::getApplication());
	}

	object->postInitProperties();
	object->onCreated();
}

bool isValid(const Object* object) noexcept {
	if(object == nullptr){
		return false;
	}

	return ObjectManager::get()->isValid(object);
}