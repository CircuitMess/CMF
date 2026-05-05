#ifndef CMF_RM_MOTIONSENSOR_H
#define CMF_RM_MOTIONSENSOR_H

#include "Object/Class.h"
#include "Services/Modules/ModuleDevice.h"
#include "Thread/Threaded.h"

class RM_MotionSensor : public ModuleDevice{
	GENERATED_BODY(RM_MotionSensor, ModuleDevice, CONSTRUCTOR_PACK(const Modules::BusPins&))

public:
	RM_MotionSensor(const Modules::BusPins& busPins = {});
	~RM_MotionSensor() noexcept override;
	void setLEDs(bool state);

	bool getState() const;

private:
	IRAM_ATTR static void isr(void* arg);

	std::atomic_bool active = false;

	StrongObjectPtr<Threaded> thread;
    SemaphoreHandle_t sem;
};


#endif //CMF_RM_MOTIONSENSOR_H
