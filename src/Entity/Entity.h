#ifndef CMF_ENTITY_H
#define CMF_ENTITY_H

#include "Object/Object.h"

class Entity : public Object {
	GENERATED_BODY(Entity, Object)

public:
	Entity() noexcept;
	virtual ~Entity() noexcept override;

protected:
	virtual void PostInitProperties() noexcept;
	virtual void Begin() noexcept;
	virtual void Tick(float deltaTime) noexcept;
	virtual void End(/*TODO reason*/) noexcept;
};

#endif //CMF_ENTITY_H