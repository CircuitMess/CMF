#ifndef CMF_ENTITY_H
#define CMF_ENTITY_H

#include "Object/Object.h"

/**
 * @brief 
 */
enum class EndReason : uint8_t {
	Destroyed,
	GarbageCollected,
	Quit
};

/**
 * @brief 
 */
class Entity : public Object {
	GENERATED_BODY(Entity, Object)

public:
	/**
	 * @brief 
	 */
	Entity() noexcept;

	/**
	 * @brief 
	 */
	virtual ~Entity() noexcept override;

	/**
	 * @brief 
	 * @return 
	 */
	bool hasBegun() const noexcept;

protected:
	/**
	 * @brief 
	 */
	virtual void postInitProperties() noexcept override;

	/**
	 * @brief 
	 */
	virtual void begin() noexcept;

	/**
	 * @brief 
	 * @param deltaTime 
	 */
	virtual void tick(float deltaTime) noexcept;

	/**
	 * @brief 
	 * @param reason 
	 */
	virtual void end(EndReason reason) noexcept;

private:
	bool begun;
};

#endif //CMF_ENTITY_H