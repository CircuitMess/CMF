#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"

AsyncEntity::AsyncEntity() noexcept : Super(), thread(newObject<Threaded>(this, [this]() { this->tickHandle();}, "AsyncEntityThread")), lastTickTime(micros()) {
	// TODO: replace thread name with instance object name once that is implemented
	// TODO: spec will define postInitProperties as executed in the same thread that created the entity, while all other logic functions must execute in the owned thread of the async entity
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

void AsyncEntity::onDestroy() noexcept{
	forEachChild([](Object* child){
		if(isValid(child)){
			return false;
		}

		child->setOwner(nullptr);

		return false;
	});

	thread->destroy();

	Super::onDestroy();
}

void AsyncEntity::tickHandle() noexcept{
	if(!ObjectManager::get()->isValid(this)){
		return;
	}

	const uint64_t currentTickTime = micros();
	const float deltaTime = (currentTickTime - lastTickTime) / 1000000.0f;

	if(!hasBegun()){
		begin();
	}

	scanEvents(); // TODO: time for blocked waiting etc.

	tick(deltaTime);

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

		if(!isValid(child) && !child->canDelete()){
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

	if(isMarkedForDestroy() && !canDelete()){
		onDestroy();
	}

	lastTickTime = currentTickTime;
}