#ifndef CMF_TEMPLATE_MOTORSEASER_H
#define CMF_TEMPLATE_MOTORSEASER_H

#include "Object/Object.h"

/**
 * Interface for easing the ramp-up over time of DC motors.
 */
class MotorsEaser : public Object {
	GENERATED_BODY(MotorsEaser, Object)
public:
	virtual float easeValue(float dt, float currentValue, float targetValue){
		return targetValue;
	}
};

class LinearEaser : public MotorsEaser {
	GENERATED_BODY(LinearEaser, MotorsEaser)
public:
	explicit LinearEaser(const float easeStrength = DefaultEaseStrength) : easeStrength(easeStrength){}

	float easeValue(float dt, float currentValue, float targetValue) override{
		float direction = (currentValue < targetValue) ? 1 : -1;

		return currentValue + direction * easeStrength * dt;
	}

private:
	const float easeStrength;

	static constexpr float DefaultEaseStrength = 200; //value change per second, easing speed
};

#endif //CMF_TEMPLATE_MOTORSEASER_H
