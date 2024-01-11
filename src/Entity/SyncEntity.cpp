#include "SyncEntity.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

SyncEntity::SyncEntity(AsyncEntity* owner/* = nullptr*/) noexcept : Super() {
	setOwner(owner);
}

SyncEntity::~SyncEntity() noexcept = default;

void SyncEntity::setOwner(AsyncEntity* owner) noexcept {
	AsyncEntity* oldOwner = *ownerEntity;

	if(owner == nullptr){
		ownerEntity = ApplicationStatics::getApplication();
	}else{
		ownerEntity = owner;
	}

	onOwnerChanged(oldOwner);
}

AsyncEntity* SyncEntity::getOwner() const noexcept {
	return *ownerEntity;
}

void SyncEntity::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);
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

void SyncEntity::onOwnerChanged(AsyncEntity* oldOwner) noexcept {
	oldOwner->unregisterSyncEntity(this);
	ownerEntity->registerSyncEntity(this);
}
