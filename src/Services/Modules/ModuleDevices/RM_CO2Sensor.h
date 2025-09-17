#ifndef CMF_RM_CO2SENSOR_H
#define CMF_RM_CO2SENSOR_H

#include "Services/Modules/ModuleDevice.h"
#include "Services/ADCReader.h"

class RM_CO2Sensor : public ModuleDevice{
	GENERATED_BODY(RM_CO2Sensor, ModuleDevice)
public:
	RM_CO2Sensor(const Modules::BusPins& pins = {});

	uint32_t getReading();

	bool isOK();
private:
	static constexpr uint32_t OKThreshold = 2100;
	StrongObjectPtr<ADCReader> adc_pin;
};


#endif //CMF_RM_CO2SENSOR_H
