#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"

AsyncEntity::AsyncEntity() noexcept : Super(), thread(newObject<Threaded>(this, [this]() { this->tickHandle();}, "AsyncEntityThread")), lastTickTime(micros()) {
	// TODO: replace thread name with instance object name once that is implemented
	// TODO: ensure that begin, tick and end are all done in the same thread
	// TODO: spec will define postInitProperties as executed in the same thread that created the entity, while all other logic functions must execute in the owned thread of the async entity
	// TODO: extend Threaded and encapsulate that class to ensure the spec above
}

AsyncEntity::~AsyncEntity() noexcept {
	if(thread == nullptr){
		return;
	}

	thread->stop(portMAX_DELAY);
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

	forEachChild([deltaTime](Object* child) {
		if(child == nullptr){
			return false;
		}

		child->scanEvents(); // TODO: time for blocked waiting etc.

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			entity->tick(deltaTime);
		}

		return false;
	});

	lastTickTime = currentTickTime;
}