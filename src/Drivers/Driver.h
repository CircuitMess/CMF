#ifndef CMF_DRIVER_H
#define CMF_DRIVER_H

#include "Misc/Enum.h"

class Driver {
public:
	virtual void scan() noexcept = 0;
	virtual float read(Enum<int> port) noexcept = 0;
	virtual bool getState(Enum<int> port) noexcept = 0;
	virtual void write(Enum<int> port, float value) noexcept = 0;
	virtual void send() noexcept = 0;
};

#endif //CMF_DRIVER_H
