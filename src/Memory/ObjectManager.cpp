#include "ObjectManager.h"

uint32_t ObjectManager::getReferenceCount(Object* object) const noexcept{
	if(object == nullptr){
		return 0;
	}

	return objectReferenceTree[object].count;
}

bool ObjectManager::isValid(Object* object) const noexcept{
	if(object == nullptr){
		return false;
	}

	return getReferenceCount(object) > 0; // TODO: && object not marked for destroy
}

void ObjectManager::registerReference(Object* object) noexcept{
	// TODO: not implemented
}

void ObjectManager::unregisterReference(Object* object) noexcept{
	// TODO: not implemented
}
