#ifndef CMF_LEDBLINKFUNCTION_H
#define CMF_LEDBLINKFUNCTION_H

#include "LED.h"
#include <Drivers/Interface/InputDriver.h>

template<typename LED, typename DataT>
class LEDBlinkFunction : public LEDFunction<LED, DataT> {
	TEMPLATE_ATTRIBUTES(LED, DataT)
	GENERATED_BODY(LEDBlinkFunction, TEMPLATED_TYPE(LEDFunction<LED, DataT>))
public:
	LEDBlinkFunction() = default;

	/**
	 * @param value - ON value for blinking
	 * @param period - On/Off blink period in seconds
	 * @param count - number of blink cycles to be repeated, or 0 for infinite blinks
	 */
	LEDBlinkFunction(DataT value, float period, uint32_t count = 0) : value(value), count(count), period(period){
	}

	DataT getValue() const noexcept override{
		if(state) return value;

		return DataT{};
	}

	bool isDone() const noexcept override{
		if(count != 0 && elapsedCount >= count){
			printf("done\n");
			return true;
		}

		return false;
	}

private:
	void tick(float deltaTime) noexcept override{
		timer += deltaTime;

		state = false;
		if(fmodf(timer, period) <= period / 2){
			state = true;
		}

		elapsedCount = timer / period;
	}

	DataT value;
	uint32_t count;
	float period;

	float timer = 0;
	uint32_t elapsedCount = 0;
	bool state = false;
};

#endif //CMF_LEDBLINKFUNCTION_H
