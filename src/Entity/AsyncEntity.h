#ifndef CMF_ASYNCENTITY_H
#define CMF_ASYNCENTITY_H

#include "Entity.h"
#include "Object/Class.h"
#include <set>
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Thread/Threaded.h"

class SyncEntity;

/**
 * @brief AsyncEntity is an owner-less Entity implementation which begins,
 * ticks and ends asynchronously in its own thread,
 * and handles lifetime of its child objects in the same thread.
 */
class AsyncEntity : public Entity {
	GENERATED_BODY(AsyncEntity, Entity, CONSTRUCTOR_PACK(TickType_t, size_t, uint8_t, int8_t))

public:
	/**
	 * @brief Creates an async entity with given parameters as its behavior properties.
	 * @param interval The time between ticks.
	 * @param threadStackSize Stack size of the async entity thread.
	 * @param threadPriority Thread priority.
	 * @param cpuCore The core of execution.
	 */
	explicit AsyncEntity(TickType_t interval = 0, size_t threadStackSize = 4 * 1024, uint8_t threadPriority = 5, int8_t cpuCore = -1) noexcept;

	/**
	 * @brief Stops the thread if still running, then destroys the entity.
	 */
	virtual ~AsyncEntity() noexcept override;

protected:
	/**
	 * @brief Creates the thread of the entity and starts its execution.
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
	 * @brief Ensures that the owner set is always nullptr since async entities cannot have an owner.
	 * @param object The owner object, always ignored and set to nullptr.
	 */
	virtual void setOwner(Object* object) noexcept override final;

	/**
	 * @brief Stops and destroys the thread.
	 */
	virtual void onDestroy() noexcept override;

private:
	/**
	 * @brief The internal tick handle which is running within the native thread.
	 * This function calls the tick and event scanning of this object,
	 * as well as all necessary lifetime and event functionality of child objects and entities.
	 */
	void tickHandle() noexcept;

private:
	StrongObjectPtr<Threaded> thread;
	size_t threadStackSize;
	uint8_t threadPriority;
	int8_t cpuCore;
	uint64_t lastTickTime;
	TickType_t tickingInterval;
};

#endif //CMF_ASYNCENTITY_H