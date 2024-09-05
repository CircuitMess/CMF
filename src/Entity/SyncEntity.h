#ifndef CMF_SYNCENTITY_H
#define CMF_SYNCENTITY_H

#include "Entity.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

class AsyncEntity;

class SyncEntity : public Entity {
	GENERATED_BODY(SyncEntity, Entity)

public:
	explicit SyncEntity() noexcept;
	virtual ~SyncEntity() noexcept override;

	virtual void tick(float deltaTime) noexcept override;
	virtual void begin() noexcept override;
	virtual void end(EndReason reason) noexcept override;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void onOwnerChanged(Object* oldOwner) noexcept override;

private:
	// TODO remove or change to Object
	WeakObjectPtr<AsyncEntity> ownerEntity;
};

#endif //CMF_SYNCENTITY_H