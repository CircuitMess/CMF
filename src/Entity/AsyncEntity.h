#ifndef CMF_ASYNCENTITY_H
#define CMF_ASYNCENTITY_H

#include "Entity.h"
#include <set>
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Thread/Threaded.h"

class SyncEntity;

class AsyncEntity : public Entity {
	GENERATED_BODY(AsyncEntity, Entity)

public:
	AsyncEntity() noexcept;
	virtual ~AsyncEntity() noexcept override;

	void registerSyncEntity(SyncEntity* entity) noexcept;
	void unregisterSyncEntity(SyncEntity* entity) noexcept;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(/*TODO reason*/) noexcept override;

private:
	void tickHandle() noexcept;

private:
	std::set<WeakObjectPtr<SyncEntity>> registeredSyncEntities;
	StrongObjectPtr<Threaded> thread;
	uint64_t lastTickTime;
};

#endif //CMF_ASYNCENTITY_H