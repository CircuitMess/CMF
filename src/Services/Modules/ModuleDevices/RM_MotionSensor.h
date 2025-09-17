#ifndef CMF_RM_MOTIONSENSOR_H
#define CMF_RM_MOTIONSENSOR_H

#include "Services/Modules/ModuleDevice.h"

class RM_MotionSensor : public ModuleDevice{
	GENERATED_BODY(RM_MotionSensor, ModuleDevice)
public:
	RM_MotionSensor(const Modules::BusPins& busPins = {});

	void setLEDs(bool state);

	//TODO - readanje ili eventi?
	bool getState() const;

private:
	IRAM_ATTR static void isr(void* arg);

	std::atomic_bool active = false;
};


#endif //CMF_RM_MOTIONSENSOR_H
