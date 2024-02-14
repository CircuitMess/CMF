#ifndef CMF_ENTITY_H
#define CMF_ENTITY_H

#include "Object/Object.h"

// TODO: still need to implement logic for passing quit reason to end

enum class EndReason : uint8_t {
	Destroyed,
	GarbageCollected,
	Quit
};

class Entity : public Object {
	GENERATED_BODY(Entity, Object)

public:
	Entity() noexcept;
	virtual ~Entity() noexcept override;

	bool hasBegun() const noexcept;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept;
	virtual void tick(float deltaTime) noexcept;
	virtual void end(EndReason reason) noexcept;

private:
	bool begun = false;
};

#endif //CMF_ENTITY_H