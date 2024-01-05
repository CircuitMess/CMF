#ifndef CMF_SYNCENTITY_H
#define CMF_SYNCENTITY_H

#include "Entity.h"

class AsyncEntity;

class SyncEntity : public Entity {
	GENERATED_BODY(SyncEntity, Entity)

public:
	SyncEntity(AsyncEntity* owner = nullptr) noexcept;
	virtual ~SyncEntity() noexcept override;

	void SetOwner(AsyncEntity* owner = nullptr) noexcept;
	AsyncEntity* GetOwner() const noexcept;

protected:
	virtual void PostInitProperties() noexcept override;
	virtual void Begin() noexcept override;
	virtual void Tick(float deltaTime) noexcept override;
	virtual void End(/*TODO reason*/) noexcept override;

	virtual void OnOwnerChanged(AsyncEntity* oldOwner) noexcept;

private:
	AsyncEntity* m_Owner = nullptr;
};

#endif //CMF_SYNCENTITY_H