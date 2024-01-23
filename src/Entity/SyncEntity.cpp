#include "SyncEntity.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"
#include "Memory/ObjectMemory.h"

SyncEntity::SyncEntity(Object* owner/* = nullptr*/) noexcept : Super() {
	if(owner == nullptr){
		setOwner(ApplicationStatics::getApplication());
	}else {
		setOwner(owner);
	}
}

SyncEntity::~SyncEntity() noexcept = default;

void SyncEntity::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);

	forEachChild([](Object* child) {
		if(!isValid(child)){
			return false;
		}

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			entity->begin();
		}

		return false;
	});

	forEachChild([deltaTime](Object* child) {
		if(!isValid(child)){
			return false;
		}

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			entity->tick(deltaTime);
		}

		return false;
	});

	std::set<Object*> childrenToRemove;
	forEachChild([&childrenToRemove](Object* child) {
		if(!isValid(child)){
			return false;
		}

		if(child->isMarkedForDestroy() && !child->canDelete()){
			if(SyncEntity* entity = cast<SyncEntity>(child)){
				entity->end();
			}
			childrenToRemove.insert(child);
		}

		return false;
	});

	for(Object* child : childrenToRemove){
		if(child == nullptr){
			continue;
		}

		child->setOwner(nullptr);
		child->onDestroy();
	}
}

void SyncEntity::postInitProperties() noexcept {
	Super::postInitProperties();
}

void SyncEntity::begin() noexcept {
	Super::begin();
}

void SyncEntity::end() noexcept {
	Super::end();
}
