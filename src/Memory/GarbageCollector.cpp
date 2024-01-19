#include "GarbageCollector.h"
#include "ObjectManager.h"

GarbageCollector::GarbageCollector() noexcept{
	// TODO: configure tick frequency of async entity
}

void GarbageCollector::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	std::set<Object*> objectsToDelete;

	ObjectManager::get()->forEachObject([&objectsToDelete](Object* object) -> bool {
		if(ObjectManager::get()->isValid(object)){
			objectsToDelete.insert(object);
		}

		return false;
	});

	for(Object* object : objectsToDelete){
		if(object == nullptr){
			continue;
		}

		if(object->canDelete() || ObjectManager::get()->getReferenceCount(object) == 0){
			ObjectManager::get()->onObjectDeleted(object);

			delete object;
		}
	}
}
