#ifndef CMF_GARBAGECOLLECTOR_H
#define CMF_GARBAGECOLLECTOR_H

#include "Entity/AsyncEntity.h"

/**
 * @brief Garbage collector goes through all objects in memory and frees the memory of those that do not have any strong references to them, or those that are marked for deletion.
 * The garbage collector checks every object in each tick, by default, ticking every 120 seconds for performance reasons.
 * The faster the garbage collector ticks, the better the memory management, but the worse the performance of the application.
 */
class GarbageCollector : public AsyncEntity {
	GENERATED_BODY(GarbageCollector, AsyncEntity)

public:
	/**
	 * @brief Default constructor, sets the tick interval.
	 */
	GarbageCollector() noexcept;

	/**
	 * @brief Default destructor.
	 */
	virtual ~GarbageCollector() noexcept override = default;

protected:
	/**
	 * @brief Checks which objects are ready to be deleted, and deletes them.
	 * @param deltaTime How much time has passed since the last tick call.
	 */
	virtual void tick(float deltaTime) noexcept override;
};

#endif //CMF_GARBAGECOLLECTOR_H