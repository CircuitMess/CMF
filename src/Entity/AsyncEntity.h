#ifndef CMF_ASYNCENTITY_H
#define CMF_ASYNCENTITY_H

#include "Entity.h"
#include <set>

class SyncEntity;

class AsyncEntity : public Entity {
	GENERATED_BODY(AsyncEntity, Entity)

public:
	AsyncEntity() noexcept;
	virtual ~AsyncEntity() noexcept override;

	void RegisterSyncEntity(SyncEntity* entity) noexcept;
	void UnregisterSyncEntity(SyncEntity* entity) noexcept;

protected:
	virtual void PostInitProperties() noexcept override;
	virtual void Begin() noexcept override;
	virtual void Tick(float deltaTime) noexcept override;
	virtual void End(/*TODO reason*/) noexcept override;

private:
	void TickHandle() noexcept;

private:
	std::set<SyncEntity*> m_RegisteredSyncEntities; // TODO: replace these with weak ptrs
};

#endif //CMF_ASYNCENTITY_H