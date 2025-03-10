#ifndef CMF_MOTORS_H
#define CMF_MOTORS_H

#include "Entity/AsyncEntity.h"
#include "Drivers/Interface/OutputDriver.h"
#include "MotorsEaser.h"
#include "Drivers/Output/OutputPWM.h"
#include <cmath>

template<typename Motor>
struct MotorDef {
	Motor motor;
	OutputPin digitalPin;
	OutputPin analogPin;
};

DEFINE_LOG(Motors)

template<typename Motor>
class Motors : public AsyncEntity {
	GENERATED_BODY(Motors, AsyncEntity)
	TEMPLATE_ATTRIBUTES(Motor)
public:

	Motors() = default;

	Motors(const std::vector<MotorDef<Motor>>& motors, Object* easer = newObject<MotorsEaser>().get()) : Super(0, 3 * 1024, 8, 1), motorDefs(motors){
		if(!easer->isA(MotorsEaser::staticClass())){
			CMF_LOG(Motors, LogLevel::Error, "Easer parameter isn't a MotorsEaser instance!");
			this->easer = newObject<MotorsEaser>().get();
			return;
		}

		this->easer = easer;

		outputSemaphore = xSemaphoreCreateBinary();
		xSemaphoreTake(outputSemaphore, 0);
	}

	void reg(MotorDef<Motor> motor){
		motor.digitalPin.driver->write(motor.digitalPin.port, false);
		motor.analogPin.driver->write(motor.analogPin.port, false);

		motorPins[motor.motor] = { motor.digitalPin, motor.analogPin };
		currentValues[motor.motor] = 0;
		targetValues[motor.motor] = 0;

	}

	/**
	 * Sets the specified motor to val
	 * @param motor
	 * @param val [-100, 100]
	 */
	void set(Motor motor, float val){
		val = std::clamp(val, -100.f, 100.f);

		targetValues[motor] = val;

		xSemaphoreGive(outputSemaphore);
	}

	float get(Motor motor){
		return targetValues[motor];
	}

	void dereg(Motor motor){
		if(!motorPins.count(motor)) return;

		motorPins[motor].digitalPin.driver->write(motorPins[motor].digitalPin.port, false);
		motorPins[motor].analogPin.driver->write(motorPins[motor].analogPin.port, false);

		motorPins.erase(motor);
		currentValues.erase(motor);
		targetValues.erase(motor);
	}

protected:
	void tick(float deltaTime) noexcept override{
		Super::tick(deltaTime);

		if(!xSemaphoreTake(outputSemaphore, portMAX_DELAY)) {
			return;
		}

		for(const auto& motorPin: motorPins){

			const Motor motor = motorPin.first;

			if(std::round(currentValues[motor]) == std::round(targetValues[motor])) continue;


			float oldValue = currentValues[motor];
			float newValue = std::clamp(easer->easeValue(deltaTime, currentValues[motor], targetValues[motor]), -100.f, 100.f);


			currentValues[motor] = newValue;

			if(std::round(newValue) != std::round(oldValue)){
				const MotorPins& pins = motorPin.second;

				if(newValue == 0){
					pins.digitalPin.driver->write(pins.digitalPin.port, true);
					pins.analogPin.driver->write(pins.analogPin.port, true);
					continue;
				}

				// motor driver is a basic H bridge and has two input lines (A and B) for each motor
				// Ab - forward, aB - backward, AB - brake, ab - no power
				// going forward: pull B low, drive A with PWM
				// going backward: put B high, drive A with reverse PWM value

				// value is [-100, 100], duty is [0, 100]
				const bool reverse = (newValue < 0);
				float duty = abs(newValue);

				if(reverse){
					pins.digitalPin.driver->write(pins.digitalPin.port, true);
					duty = 100.f - duty;
				}else{
					pins.digitalPin.driver->write(pins.digitalPin.port, false);
				}

				pins.analogPin.driver->write(pins.analogPin.port, duty / 100.f);
			}
		}
	}

	void postInitProperties() noexcept override{
		Super::postInitProperties();

		for(const auto& motor: motorDefs){
			reg(motor);
		}
	}

private:
	StrongObjectPtr<MotorsEaser> easer = nullptr;

	const std::vector<MotorDef<Motor>> motorDefs;

	/**
	 * Map of current motor values.
	 * key = Motor[enum, int] , value = state[float]
	 */
	std::map<Motor, float> currentValues;

	std::map<Motor, float> targetValues;

	struct MotorPins {
		OutputPin digitalPin;
		OutputPin analogPin;
	};

	std::map<Motor, MotorPins> motorPins;

	SemaphoreHandle_t outputSemaphore;
};

#endif //CMF_MOTORS_H