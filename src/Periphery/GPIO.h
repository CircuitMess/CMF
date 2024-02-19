#ifndef CMF_GPIO_H
#define CMF_GPIO_H

#include "Entity/SyncEntity.h"

enum class Pin : int8_t {
	None = -1,
	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,
	Num10,
	Num11,
	Num12,
	Num13,
	Num14,
	Num15,
	Num16,
	Num17,
	Num18,
	Num19,
	Num20,
	Num21,
	Num22,
	Num23,
	Num24,
	Num25,
	Num26,
	Num27,
	Num28,
	Num29,
	Num30,
	Num31,
	Num32,
	Num33,
	Num34,
	Num35,
	Num36,
	Num37,
	Num38,
	Num39,
	Num40,
	Num41,
	Num42,
	Num43,
	Num44,
	Num45,
	Num46,
	Num47,
	Num48,
	COUNT
};

class GPIO : public SyncEntity {
	GENERATED_BODY(GPIO, SyncEntity)

public:
};

#endif //CMF_GPIO_H