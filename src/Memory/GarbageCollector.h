#ifndef CMF_GARBAGECOLLECTOR_H
#define CMF_GARBAGECOLLECTOR_H

#include "Entity/AsyncEntity.h"

class GarbageCollector : public AsyncEntity {
	GENERATED_BODY(GarbageCollector, AsyncEntity)

public:
	GarbageCollector() noexcept;
	virtual ~GarbageCollector() noexcept = default;

protected:
	virtual void tick(float deltaTime) noexcept override;
};

#endif //CMF_GARBAGECOLLECTOR_H