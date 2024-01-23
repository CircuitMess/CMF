#ifndef CMF_SYNCENTITY_H
#define CMF_SYNCENTITY_H

#include "Entity.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

class AsyncEntity;

class SyncEntity : public Entity {
	GENERATED_BODY(SyncEntity, Entity)

public:
	explicit SyncEntity(Object* owner = nullptr) noexcept;
	virtual ~SyncEntity() noexcept override;

	virtual void tick(float deltaTime) noexcept override;
	virtual void begin() noexcept override;
	virtual void end(/*TODO reason*/) noexcept override;

protected:
	virtual void postInitProperties() noexcept override;

private:
	WeakObjectPtr<AsyncEntity> ownerEntity = nullptr;
};

#endif //CMF_SYNCENTITY_H