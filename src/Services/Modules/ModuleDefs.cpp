#include "ModuleDefs.h"
#include <map>

#ifdef CONFIG_RM_TempHum

#include "ModuleDevices/RM_TempHumModule.h"

#endif
#ifdef CONFIG_RM_LED

#include "ModuleDevices/RM_LEDModule.h"

#endif
#ifdef CONFIG_RM_Motion

#include "ModuleDevices/RM_MotionSensor.h"

#endif
#ifdef CONFIG_RM_CO2

#include "ModuleDevices/RM_CO2Sensor.h"

#endif
#ifdef CONFIG_RM_IR

#include "ModuleDevices/RM_IRModule.h"

#endif
#ifdef CONFIG_RM_PerfBoard

#include "ModuleDevices/RM_Perfboard.h"

#endif


const std::map<ModuleAddress, ModuleType>& GetAddressMap(){
	static const std::map<ModuleAddress, ModuleType> addressMap = {
#ifdef CONFIG_RM_TempHum
			{{ 61, { .type=ModuleSubAddress::Type::RM_I2C, .I2CAddress=0x38 }}, ModuleType::RM_TempHum },
#endif
#ifdef CONFIG_RM_LED
			{{ 16, { .type=ModuleSubAddress::Type::RM, .RMAddress=3 }}, ModuleType::RM_LED },
#endif
#ifdef CONFIG_RM_Motion
			{{ 16, { .type=ModuleSubAddress::Type::RM, .RMAddress=0 }}, ModuleType::RM_Motion },
#endif
#ifdef CONFIG_RM_CO2
			{{ 16, { .type=ModuleSubAddress::Type::RM, .RMAddress=1 }}, ModuleType::RM_CO2 },
#endif
#ifdef CONFIG_RM_IR
			{{ 16, { .type=ModuleSubAddress::Type::RM, .RMAddress=2 }}, ModuleType::RM_IR },
#endif
#ifdef CONFIG_RM_PerfBoard
			{{ 16, { .type=ModuleSubAddress::Type::RM, .RMAddress=4 }}, ModuleType::RM_PerfBoard },
#endif
	};
	return addressMap;
}

const std::map<ModuleType, SubclassOf<ModuleDevice>>& GetDeviceMap(){
	static const std::map<ModuleType, SubclassOf<ModuleDevice>> deviceMap = {
#ifdef CONFIG_RM_TempHum
			{ ModuleType::RM_TempHum, RM_TempHumModule::staticClass() },
#endif
#ifdef CONFIG_RM_LED
			{ ModuleType::RM_LED, RM_LEDModule::staticClass() },
#endif
#ifdef CONFIG_RM_Motion
			{ ModuleType::RM_Motion, RM_MotionSensor::staticClass() },
#endif
#ifdef CONFIG_RM_CO2
			{ ModuleType::RM_CO2, RM_CO2Sensor::staticClass() },
#endif
#ifdef CONFIG_RM_IR
			{ ModuleType::RM_IR, RM_IRModule::staticClass() },
#endif
#ifdef CONFIG_RM_PerfBoard
			{ ModuleType::RM_PerfBoard, RM_Perfboard::staticClass() },
#endif
	};
	return deviceMap;
}

const std::map<uint8_t, std::set<ModuleSubAddress>> GetSubAddressMap(){
	static std::map<uint8_t, std::set<ModuleSubAddress>> SubAddressMap;
	for(const auto& it:GetAddressMap()){
		const auto& address = it.first;
		SubAddressMap[address.mainAddress].insert(address.subAddress);
	}
	return SubAddressMap;
}

