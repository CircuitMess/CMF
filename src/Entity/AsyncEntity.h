#ifndef CMF_ASYNCENTITY_H
#define CMF_ASYNCENTITY_H

#include "Entity.h"

class AsyncEntity : public Entity {
	GENERATED_BODY(AsyncEntity, Entity)

public:
	AsyncEntity() noexcept;
	virtual ~AsyncEntity() noexcept override;

protected:
	virtual void PostInitProperties() noexcept override;
	virtual void Begin() noexcept override;
	virtual void Tick(float deltaTime) noexcept override;
	virtual void End(/*TODO reason*/) noexcept override;
};

#endif //CMF_ASYNCENTITY_H