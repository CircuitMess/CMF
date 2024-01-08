#ifndef CMF_SYNCENTITY_H
#define CMF_SYNCENTITY_H

#include "Entity.h"

class AsyncEntity;

class SyncEntity : public Entity {
	GENERATED_BODY(SyncEntity, Entity)

public:
	explicit SyncEntity(AsyncEntity* owner = nullptr) noexcept;
	virtual ~SyncEntity() noexcept override;

	void setOwner(AsyncEntity* owner = nullptr) noexcept;
	AsyncEntity* getOwner() const noexcept;

	virtual void tick(float deltaTime) noexcept override;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void end(/*TODO reason*/) noexcept override;

	virtual void onOwnerChanged(AsyncEntity* oldOwner) noexcept;

private:
	AsyncEntity* m_Owner = nullptr;
};

#endif //CMF_SYNCENTITY_H