#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Thread/Threaded.h"

AsyncEntity::AsyncEntity() noexcept : Super() {
	thread = new Threaded([this]() { this->tickHandle();}, "AsyncEntityThread"); // TODO: relpace thread name with instance object name once that is implemented
	thread->start();
}

AsyncEntity::~AsyncEntity() noexcept {
	thread->stop(portMAX_DELAY);
	delete thread;
}

void AsyncEntity::registerSyncEntity(SyncEntity* entity) noexcept{
	if(entity->getOwner() != this){
		entity->setOwner(this);
	}

	registeredSyncEntities.insert(entity);
}

void AsyncEntity::unregisterSyncEntity(SyncEntity* entity) noexcept {
	if(entity->getOwner() == this){
		entity->setOwner(nullptr);
	}

	registeredSyncEntities.erase(entity);
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

void AsyncEntity::tickHandle() noexcept{
	tick(0.0f); // TODO: delta time

	for(SyncEntity* entity : registeredSyncEntities){
		if(entity == nullptr){ // TODO: check ptr validity instead of nullptr
			registeredSyncEntities.erase(nullptr);
		}else{
			entity->tick(0.0f); // TODO: delta time
		}
	}
}