#include "AsyncEntity.h"
#include "SyncEntity.h"

AsyncEntity::AsyncEntity() noexcept : Super() {

}

AsyncEntity::~AsyncEntity() noexcept {

}

void AsyncEntity::registerSyncEntity(SyncEntity* entity) noexcept{
	if(entity->getOwner() != this){
		entity->setOwner(this);
	}

	m_RegisteredSyncEntities.insert(entity);
}

void AsyncEntity::unregisterSyncEntity(SyncEntity* entity) noexcept {
	if(entity->getOwner() == this){
		entity->setOwner(nullptr);
	}

	m_RegisteredSyncEntities.erase(entity);
}

void AsyncEntity::postInitProperties() noexcept {
	Super::postInitProperties();
}

void AsyncEntity::begin() noexcept {
	Super::begin();
}

void AsyncEntity::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);
}

void AsyncEntity::end() noexcept {
	Super::end();
}

void AsyncEntity::TickHandle() noexcept{
	tick(0.0f); // TODO: delta time

	for(SyncEntity* entity : m_RegisteredSyncEntities){
		if(entity != nullptr){ // TODO: check ptr validity instead of nullptr
			entity->tick(0.0f); // TODO: delta time
		}
	}
}