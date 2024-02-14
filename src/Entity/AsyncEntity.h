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
	explicit AsyncEntity(TickType_t interval = 0, size_t threadStackSize = 4 * 1024, uint8_t threadPriority = 5, int8_t cpuCore = -1) noexcept;
	virtual ~AsyncEntity() noexcept override;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(EndReason reason) noexcept override;
	virtual void setOwner(Object* object) noexcept override final;
	virtual void onDestroy() noexcept override;
	virtual TickType_t getEventScanningTime() const noexcept override;
	virtual TickType_t getTickingInterval() const noexcept;

	void setTickingInterval(TickType_t value) noexcept;

private:
	void tickHandle() noexcept;

private:
	StrongObjectPtr<Threaded> thread;
	TickType_t interval;
	size_t threadStackSize;
	uint8_t threadPriority;
	int8_t cpuCore;
	uint64_t lastTickTime;
	TickType_t tickingInterval;
};

#endif //CMF_ASYNCENTITY_H