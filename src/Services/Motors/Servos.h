#ifndef CMF_TEMPLATE_SERVOS_H
#define CMF_TEMPLATE_SERVOS_H

#include "Drivers/Interface/OutputDriver.h"
#include "Entity/AsyncEntity.h"
#include "Drivers/Output/OutputMCPWM.h"
#include "Drivers/Output/OutputPWM.h"

DEFINE_LOG(Servos)

struct ServoDef {
	int motor;
	OutputPin pin;
};

template<typename Servo>
class Servos : public AsyncEntity {
	GENERATED_BODY(Servos, AsyncEntity);
	TEMPLATE_ATTRIBUTES(Servo)
public:
	Servos() = default;

	Servos(std::vector<ServoDef> servos, Object* easer = newObject<MotorsEaser>().get()) : AsyncEntity(10 / portTICK_PERIOD_MS, 3 * 1024), servoDefs(servos){
		if(!easer->isA(MotorsEaser::staticClass())){
			CMF_LOG(Motors, LogLevel::Error, "Easer parameter isn't a MotorsEaser instance!");
			this->easer = newObject<MotorsEaser>().get();
			return;
		}

		this->easer = easer;
	}

	void reg(Servo servo, OutputPin pin){
		pins[servo] = pin;
		currentValues[servo] = 0;
		targetValues[servo] = 0;

		pin.driver->write(pin.port, false);
	}

	/**
	 * Set servo value.
	 * @param servo
	 * @param val [0 - 1.0]
	 */
	void set(Servo servo, float val){
		if(!targetValues.count(servo)) return;

		val = std::clamp(val, 0.f, 1.f);

		targetValues[servo] = val;
	}

	float get(Servo servo){
		if(!targetValues.count(servo)) return 0;

		return targetValues[servo];
	}

	void enable(Servo servo){
		if(!pins.count(servo)) return;

		const auto& pin = pins[servo];

		if(pin.driver->isA(OutputPWM::staticClass())){
			cast<OutputPWM>(pin.driver)->attach(pin.port);
		}else if(pin.driver->isA(OutputMCPWM::staticClass())){
			cast<OutputMCPWM>(pin.driver)->enable(pin.port);
		}else{
			CMF_LOG(Servos, LogLevel::Warning, "Underlying output driver doesn't support enable/disable");
		}
	}

	void disable(Servo servo){
		if(!pins.count(servo)) return;

		const auto& pin = pins[servo];

		if(pin.driver->isA(OutputPWM::staticClass())){
			cast<OutputPWM>(pin.driver)->detach(pin.port);
		}else if(pin.driver->isA(OutputMCPWM::staticClass())){
			cast<OutputMCPWM>(pin.driver)->disable(pin.port);
		}else{
			CMF_LOG(Servos, LogLevel::Warning, "Underlying output driver doesn't support enable/disable");
		}
	}

protected:
	void tick(float deltaTime) noexcept override{
		Super::tick(deltaTime);

		for(const auto& motorPin: pins){

			const Servo motor = motorPin.first;

			if(fabs(currentValues[motor] - targetValues[motor]) < 0.01) continue;

			float oldValue = currentValues[motor];
			float newValue = std::clamp(easer->easeValue(deltaTime, currentValues[motor], targetValues[motor]), 0.f, 1.f);

			currentValues[motor] = newValue;

			if(fabs(newValue - oldValue) > 0.01){
				const auto& pin = motorPin.second;

				pin.driver->write(pin.port, newValue);
			}
		}
	}

	void postInitProperties() noexcept override{
		Super::postInitProperties();

		for(const auto& motor: servoDefs){
			reg((Servo) motor.motor, motor.pin);
		}
	}

private:
	StrongObjectPtr<MotorsEaser> easer = nullptr;

	const std::vector<ServoDef> servoDefs;

	/**
	 * Map of current motor values.
	 * key = Motor[enum, int] , value = state[float]
	 */
	std::map<Servo, float> currentValues;

	std::map<Servo, float> targetValues;

	std::map<Servo, OutputPin> pins;
};


#endif //CMF_TEMPLATE_SERVOS_H
