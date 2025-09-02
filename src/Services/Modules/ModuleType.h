#ifndef CMF_MODULETYPE_H
#define CMF_MODULETYPE_H

enum class ModuleType : uint8_t {
	Unknown = 0,
#ifdef CONFIG_RM_TempHum
	RM_TempHum,
#endif
#ifdef CONFIG_RM_LED
	RM_LED,
#endif
#ifdef CONFIG_RM_Motion
	RM_Motion,
#endif
#ifdef CONFIG_RM_CO2
	RM_CO2,
#endif
#ifdef CONFIG_RM_IR
	RM_IR,
#endif
#ifdef CONFIG_RM_PerfBoard
	RM_PerfBoard,
#endif
};
#endif //CMF_MODULETYPE_H
