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

	forEachChild([deltaTime](Object* child){
		if(isValid(child)){
			return false;
		}

		if(SyncEntity* entityChild = cast<SyncEntity>(child)){
			entityChild->tick(deltaTime);
		}

		return false;
	});
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
