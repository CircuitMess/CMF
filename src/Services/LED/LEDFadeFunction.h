#ifndef CMF_LEDFADEFUNCTION_H
#define CMF_LEDFADEFUNCTION_H

#include "LED.h"

template<typename LED, typename DataT>
class LEDFadeFunction : public LEDFunction<LED, DataT> {
	TEMPLATE_ATTRIBUTES(LED, DataT)
	GENERATED_BODY(LEDFadeFunction, TEMPLATED_TYPE(LEDFunction<LED, DataT>))
public:
	LEDFadeFunction() = default;

	LEDFadeFunction(DataT start, DataT end, float period, uint32_t loopsCount) : start(start), end(end), period(period), count(loopsCount){}

	DataT getValue() const noexcept override{
		float t = 0.5 * cos(2 * M_PI * timeCounter / period) + 0.5;

		return start * (1 - t) + end * t;

	}

	bool isDone() const noexcept override{
		if(count != 0 && elapsedCount >= count){
			return true;
		}

		return false;
	}

private:
public:
	void tick(float deltaTime) noexcept override{
		SyncEntity::tick(deltaTime);

		timeCounter += deltaTime;

		if(timeCounter > period){
			elapsedCount++;
			timeCounter = fmod(timeCounter, period);
		}
	}

private:

	DataT start;
	DataT end;
	float period;
	uint32_t count;
	uint32_t elapsedCount = 0;

	float timeCounter;
};


#endif //CMF_LEDFADEFUNCTION_H
