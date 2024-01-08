#include "SyncEntity.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

SyncEntity::SyncEntity(AsyncEntity* owner/* = nullptr*/) noexcept : Super() {
	setOwner(owner);
}

SyncEntity::~SyncEntity() noexcept {
	m_Owner = nullptr;
}

void SyncEntity::setOwner(AsyncEntity* owner) noexcept {
	AsyncEntity* oldOwner = m_Owner;

	if(owner == nullptr){
		m_Owner = ApplicationStatics::getApplication();
	}else{
		m_Owner = owner;
	}

	onOwnerChanged(oldOwner);
}

AsyncEntity* SyncEntity::getOwner() const noexcept {
	return m_Owner;
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
	m_Owner->registerSyncEntity(this);
}
