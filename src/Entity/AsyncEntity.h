#ifndef CMF_ASYNCENTITY_H
#define CMF_ASYNCENTITY_H

#include "Entity.h"
#include <set>
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Thread/Threaded.h"

class SyncEntity;

/**
 * @brief
 */
class AsyncEntity : public Entity {
	GENERATED_BODY(AsyncEntity, Entity)

public:
	/**
	 * @brief
	 * @param interval
	 * @param threadStackSize
	 * @param threadPriority
	 * @param cpuCore
	 */
	explicit AsyncEntity(TickType_t interval = 0, size_t threadStackSize = 4 * 1024, uint8_t threadPriority = 5, int8_t cpuCore = -1) noexcept;

	/**
	 * @brief
	 */
	virtual ~AsyncEntity() noexcept override;

protected:
	/**
	 * @brief
	 */
	virtual void postInitProperties() noexcept override;

	/**
	 * @brief
	 */
	virtual void begin() noexcept override;

	/**
	 * @brief
	 * @param deltaTime
	 */
	virtual void tick(float deltaTime) noexcept override;

	/**
	 * @brief
	 * @param reason
	 */
	virtual void end(EndReason reason) noexcept override;

	/**
	 * @brief
	 * @param object
	 */
	virtual void setOwner(Object* object) noexcept override final;

	/**
	 * @brief
	 */
	virtual void onDestroy() noexcept override;

	/**
	 * @brief
	 * @return
	 */
	virtual TickType_t getEventScanningTime() const noexcept override;

	/**
	 * @brief
	 * @return
	 */
	virtual TickType_t getTickingInterval() const noexcept;

	/**
	 * @brief
	 * @param value
	 */
	void setTickingInterval(TickType_t value) noexcept;

private:
	/**
	 * @brief
	 */
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