#include "AsyncEntity.h"
#include "SyncEntity.h"
#include "Util/stdafx.h"
#include "Memory/ObjectMemory.h"
#include "Log/Log.h"
#include "Core/Application.h"

AsyncEntity::AsyncEntity(TickType_t interval/* = 0*/, size_t threadStackSize/* = 12 * 1024*/, uint8_t threadPriority/* = 5*/, int8_t cpuCore/* = -1*/) noexcept :
						Super(),
						threadStackSize(threadStackSize),
						threadPriority(threadPriority),
						cpuCore(cpuCore),
						lastTickTime(micros()) {
	setEventScanningTime(interval);
}

AsyncEntity::~AsyncEntity() noexcept {
	if(!thread.isValid()){
		return;
	}

	thread->stop(portMAX_DELAY);
	// Thread will automatically get deleted as a child of this object, no need to explicitely delete it
}

void AsyncEntity::__postInitProperties() noexcept {
	Super::__postInitProperties();

	thread = newObject<Threaded>(this, [this]() { this->tickHandle();}, getName().append("_Thread"), 0, threadStackSize, threadPriority, cpuCore);
}

void AsyncEntity::setOwner(Object* object) noexcept{
	// This is on purpose, async entities should not have an owner to try to prevent accidental circular ownership and issues with event scanning
	Super::setOwner(nullptr);

	if(object != nullptr && hasBegun()){
		CMF_LOG(CMF, Warning, "Attempt to set owner '%s' of async entity '%s'. Async entities aren't allowed to have an owner.", getOwner()->getName().c_str(), getName().c_str());
	}
}

TickType_t AsyncEntity::getEventScanningTime() const noexcept {
	return eventScanningTime;
}

void AsyncEntity::setEventScanningTime(TickType_t value) noexcept {
	eventScanningTime = value;
}

void AsyncEntity::tickHandle() noexcept{
	if(!isValid(this)){
		return;
	}

	if(!hasBegun()){
		begin();
		__begin();
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
			entity->__begin();
		}

		return false;
	});

	const uint64_t currentTickTime = micros();
	const float deltaTime = (currentTickTime - lastTickTime) / 1000000.0f;

	scanEvents(getEventScanningTime());
	tick(deltaTime);
	__tick(deltaTime);

	forEachChild([deltaTime](Object* child) {
		if(!isValid(child)){
			return false;
		}

		if(SyncEntity* entity = cast<SyncEntity>(child)){
			entity->tick(deltaTime);
			entity->__tick(deltaTime);
		}

		return false;
	});

	lastTickTime = currentTickTime;
}
