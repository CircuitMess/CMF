#ifndef CMF_ENTITY_H
#define CMF_ENTITY_H

#include "Object/Object.h"

/**
 * @brief End reason enum, representing the reason why the entity is being destroyed.
 */
enum class EndReason : uint8_t {
	Destroyed,
	GarbageCollected,
	Quit
};

/**
 * @brief The base entity class representing an object that has begin, tick and end lifetime functionality.
 */
class Entity : public Object {
	GENERATED_BODY(Entity, Object)

public:
	/**
	 * @brief Creates an entity.
	 */
	Entity() noexcept;

	/**
	 * @brief Default emprt destructor.
	 */
	virtual ~Entity() noexcept override;

	/**
	 * @return True if entity has begun ticking, false otherwise.
	 */
	bool hasBegun() const noexcept;

protected:
	/**
	 * @brief 
	 */
	virtual void postInitProperties() noexcept override;

	/**
	 * @brief Called before the first tick, the function marks the begin of the entities lifetime as a fully setup object.
	 */
	virtual void begin() noexcept;

	/**
	 * @brief Called once per ticking period as long as the entity exists.
	 * @param deltaTime The time passed since the last tick call.
	 */
	virtual void tick(float deltaTime) noexcept;

	/**
	 * @brief Called after the final tick, marks the end of the entities lifetime with given reason.
	 * @param reason The reason for end of life.
	 */
	virtual void end(EndReason reason) noexcept;

private:
	bool begun;
	std::mutex beginMutex;
};

#endif //CMF_ENTITY_H