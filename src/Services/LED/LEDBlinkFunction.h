#ifndef CMF_LEDBLINKFUNCTION_H
#define CMF_LEDBLINKFUNCTION_H

#include "LED.h"

template<typename LED, typename DataT>
class LEDBlinkFunction : public LEDFunction<LED, DataT> {
public:

	LEDBlinkFunction(uint32_t count, float period) : count(count), period(period){

	}

private:
	DataT loop(float dt) override{
		timer += dt;

		bool ledState = false;
		if(fmodf(timer, period) <= period / 2){
			ledState = true;
		}

		elapsedCount = timer / period;

		return (DataT) ledState;
	}

	bool isDone() override{
		if(count != 0 && elapsedCount >= count){
			return true;
		}

		return false;
	}

	uint32_t count;
	float period;
	float timer = 0;
	uint32_t elapsedCount = 0;
};

#endif //CMF_LEDBLINKFUNCTION_H
