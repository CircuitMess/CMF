#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Thread/Threaded.h"
#include "Util/stdafx.h"

AsyncEntity::AsyncEntity() noexcept : Super(), lastTickTime(micros()) {
	thread = new Threaded([this]() { this->tickHandle();}, "AsyncEntityThread"); // TODO: replace thread name with instance object name once that is implemented
	thread->start(); // TODO: ensure that begin, tick and end are all done in the same thread
	// TODO: spec will define postInitProperties as executed in the same thread that created the entity, while all other logic functions must execute in the owned thread of the async entity
	// TODO: extend Threaded and encapsulate that class to ensure the spec above
}

AsyncEntity::~AsyncEntity() noexcept {
	if(thread == nullptr){
		return;
	}

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
	const uint64_t currentTickTime = micros();
	const float deltaTime = (currentTickTime - lastTickTime) / 1000000.0f;

	tick(deltaTime);

	for(SyncEntity* entity : registeredSyncEntities){
		if(entity == nullptr){ // TODO: check ptr validity instead of nullptr
			registeredSyncEntities.erase(nullptr);
		}else{
			entity->tick(deltaTime);
		}
	}

	lastTickTime = currentTickTime;
}