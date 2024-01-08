#ifndef CMF_ENTITY_H
#define CMF_ENTITY_H

#include "Object/Object.h"

class Entity : public Object {
	GENERATED_BODY(Entity, Object)

public:
	Entity() noexcept;
	virtual ~Entity() noexcept override;

protected:
	virtual void postInitProperties() noexcept;
	virtual void begin() noexcept;
	virtual void tick(float deltaTime) noexcept;
	virtual void end(/*TODO reason*/) noexcept;
};

#endif //CMF_ENTITY_H