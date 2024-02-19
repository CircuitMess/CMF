#ifndef CMF_DRIVER_H
#define CMF_DRIVER_H

#include "Entity/SyncEntity.h"
#include "Misc/Enum.h"

class Driver : public SyncEntity {
	GENERATED_BODY(Driver, SyncEntity);

public:
	virtual void scan() noexcept;
	virtual float read(Enum<int> port) noexcept;
	virtual bool getState(Enum<int> port) noexcept;
	virtual void write(Enum<int> port, float value) noexcept;
	virtual void send() noexcept;
};

#endif //CMF_DRIVER_H
