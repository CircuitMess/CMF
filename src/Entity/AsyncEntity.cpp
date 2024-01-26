#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"

AsyncEntity::AsyncEntity(TickType_t interval/* = 0*/, size_t threadStackSize/* = 12 * 1024*/, uint8_t threadPriority/* = 5*/, int8_t cpuCore/* = -1*/) noexcept :
						Super(),
						thread(newObject<Threaded>(this, [this]() { this->tickHandle();}, getName().append("_Thread"), interval, threadStackSize, threadPriority, cpuCore)),
						lastTickTime(micros()),
						tickingInterval(interval) {}

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
	thread->destroy();

	Super::onDestroy();
}

TickType_t AsyncEntity::getEventScanningTime() const noexcept{
	return getTickingInterval();
}

TickType_t AsyncEntity::getTickingInterval() const noexcept{
	return tickingInterval;
}

void AsyncEntity::setTickingInterval(TickType_t value) noexcept{
	tickingInterval = value;

	if(thread.isValid() && thread->getInterval() != getTickingInterval()){
		thread->setInterval(getTickingInterval());
	}
}

void AsyncEntity::tickHandle() noexcept{
	if(!ObjectManager::get()->isValid(this)){
		return;
	}

	const uint64_t currentTickTime = micros();
	const float deltaTime = (currentTickTime - lastTickTime) / 1000000.0f;

	if(thread.isValid() && thread->getInterval() != getTickingInterval()){
		thread->setInterval(getTickingInterval());
	}

	if(!hasBegun()){
		begin();
	}

	scanEvents(getEventScanningTime());

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