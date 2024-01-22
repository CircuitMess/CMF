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

	scanEvents(); // TODO: time for blocked waiting etc.

	tick(deltaTime);

	forEachChild([deltaTime](Object* child) {
		if(isValid(child)){
			return false;
		}

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			entity->tick(deltaTime);
		}

		return false;
	});

	std::set<Object*> childrenToRemove;
	forEachChild([&childrenToRemove](Object* child) {
		if(isValid(child)){
			return false;
		}

		// TODO: check what is needed for this to traverse the entire ownership tree
		if(child->isMarkedForDestroy() && !child->canDelete()){
			child->onDestroy();
			childrenToRemove.insert(child);
		}

		return false;
	});

	for(Object* child : childrenToRemove){
		if(child == nullptr){
			continue;
		}

		child->setOwner(nullptr);
	}

	if(isMarkedForDestroy() && !canDelete()){
		onDestroy();
	}

	lastTickTime = currentTickTime;
}