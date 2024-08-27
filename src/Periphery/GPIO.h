#ifndef CMF_GPIO_H
#define CMF_GPIO_H

#include "Object/Object.h"
#include "driver/gpio.h"

enum class GPIOMode : uint8_t {
	Input,                //0 - false, 1 - true
	Output,               //default state is 0
};

/**
 * Stateless wrapper for GPIO functions.
 */
class GPIO : public Object {
	GENERATED_BODY(GPIO, Object)

public:
	void setMode(gpio_num_t gpioNum, GPIOMode mode);

	void write(gpio_num_t gpioNum, bool value);

	bool read(gpio_num_t gpioNum);

	void setPullup(gpio_num_t gpioNum, bool pulledUp);

	void setPulldown(gpio_num_t gpioNum, bool pulledDown);
};

#endif //CMF_GPIO_H