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

	void registerSyncEntity(SyncEntity* entity) noexcept;
	void unregisterSyncEntity(SyncEntity* entity) noexcept;

protected:
	virtual void postInitProperties() noexcept override;
	virtual void begin() noexcept override;
	virtual void tick(float deltaTime) noexcept override;
	virtual void end(/*TODO reason*/) noexcept override;

private:
	void TickHandle() noexcept;

private:
	std::set<SyncEntity*> m_RegisteredSyncEntities; // TODO: replace these with weak ptrs
};

#endif //CMF_ASYNCENTITY_H