#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"
#include "Log/Log.h"
#include "Core/Application.h"

AsyncEntity::AsyncEntity(TickType_t interval/* = 0*/, size_t threadStackSize/* = 12 * 1024*/, uint8_t threadPriority/* = 5*/, int8_t cpuCore/* = -1*/) noexcept :
						Super(),
						interval(interval),
						threadStackSize(threadStackSize),
						threadPriority(threadPriority),
						cpuCore(cpuCore),
						lastTickTime(micros()),
						tickingInterval(interval) {}

AsyncEntity::~AsyncEntity() noexcept {
	if(!thread.isValid()){
		return;
	}

	thread->stop(portMAX_DELAY);
}

void AsyncEntity::postInitProperties() noexcept {
	Super::postInitProperties();

	thread = newObject<Threaded>(this, [this]() { this->tickHandle();}, getName().append("_Thread"), interval, threadStackSize, threadPriority, cpuCore);
}

void AsyncEntity::begin() noexcept {
	Super::begin();
}

void AsyncEntity::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);
}

void AsyncEntity::end(EndReason reason) noexcept {
	Super::end(reason);
}

void AsyncEntity::setOwner(Object* object) noexcept{
	// This is on purpose, async entities should not have an owner to try to prevent accidental circular ownership and issues with event scanning
	Super::setOwner(nullptr);

	if(object != nullptr && hasBegun()){
		CMF_LOG(LogCMF, Warning, "Attempt to set owner '%s' of async entity '%s'. Async entities aren't allowed to have an owner.", getOwner()->getName().c_str(), getName().c_str());
	}
}

void AsyncEntity::onDestroy() noexcept{
	if(thread.isValid()){
		thread->destroy();
	}

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
	if(!isValid(this)){
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

	forEachChild([](Object* child) {
		if(!isValid(child)){
			return false;
		}

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			if(entity->hasBegun()) {
				return false;
			}

			entity->begin();
		}

		return false;
	});

	scanEvents(getEventScanningTime());
	tick(deltaTime);

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
				EndReason reason = EndReason::GarbageCollected;

				if(ObjectManager::get()->getReferenceCount(entity) > 0){
					reason = EndReason::Destroyed;
				}

				entity->end(reason);
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

	if(ApplicationStatics::getApplication() != nullptr && ApplicationStatics::getApplication()->isShuttingDown()){
		forEachChild([](Object* child) {
			if(!isValid(child)){
				return false;
			}

			if(SyncEntity* entity = cast<SyncEntity>(child)){
				entity->end(EndReason::Quit);
			}

			child->destroy();
			child->onDestroy();
			child->setOwner(nullptr);

			return false;
		});
	}

	if(!isValid(this) && !canDelete()){
		EndReason reason = EndReason::GarbageCollected;

		if(ObjectManager::get()->getReferenceCount(this) > 0){
			reason = EndReason::Destroyed;
		}

		if(ApplicationStatics::getApplication() != nullptr && ApplicationStatics::getApplication()->isShuttingDown()){
			reason = EndReason::Quit;
		}

		end(reason);
		onDestroy();
	}

	lastTickTime = currentTickTime;
}