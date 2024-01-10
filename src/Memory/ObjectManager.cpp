#include "ObjectManager.h"
#include "Object/Object.h"

ObjectManager* ObjectManager::get() noexcept{
	static ObjectManager managerInstance;
	return &managerInstance;
}

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

	if(!objectReferenceTree.contains(object)){
		return false;
	}

	return !object->isMarkedForDestroy() && getReferenceCount(object) > 0;
}

void ObjectManager::registerReference(Object** object, bool keepAlive/* = false*/) noexcept{
	ObjectRefInfo& objectInfo = objectReferenceTree[*object];

	objectInfo.objectPointers.insert(object);

	if(keepAlive){
		objectInfo.count++;
	}
}

void ObjectManager::unregisterReference(Object** object, bool keepAlive/* = false*/) noexcept{
	if(!objectReferenceTree.contains(*object)){
		return;
	}

	ObjectRefInfo& objectInfo = objectReferenceTree[*object];

	objectInfo.objectPointers.erase(object);

	if(keepAlive){
		if(objectInfo.count > 0){
			objectInfo.count--;
		}

		if(!(*object)->isMarkedForDestroy() && objectInfo.count == 0){
			(*object)->destroy();
		}
	}
}
