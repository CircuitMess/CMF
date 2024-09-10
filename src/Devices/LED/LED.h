#ifndef CMF_LED_H
#define CMF_LED_H

#include "Entity/AsyncEntity.h"
#include "Misc/Enum.h"
#include "glm.hpp"
#include "Drivers/Interface/OutputDriver.h"
#include "Log/Log.h"
#include "Memory/ObjectMemory.h"

template<typename LED, typename DataT>
class LEDFunction;

template<typename LED, typename DataT> requires (std::same_as<DataT, glm::vec3> || std::same_as<DataT, float>)
class LEDBase : public SyncEntity {
	GENERATED_BODY(LEDBase, SyncEntity)

public:
	void reg(LED led, std::array<OutputPin, sizeof(DataT) / sizeof(float)> pins){
		accessMutex.lock();
		outputs[led] = pins;
		accessMutex.unlock();
		off(led);
	}

	void off(LED led){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(LogCMF, Error, "LED %d not registered", (int) led);
			return;
		}

		internalOff(led);

		prevStates[led] = DataT(); //zero-value

		if(currentFunctions.contains(led)){
			currentFunctions.erase(led);
		}
		if(prevFunctions.contains(led)){
			prevFunctions.erase(led);
		}
	}

	void on(LED led, DataT level){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(LogCMF, Error, "LED %d not registered", (int) led);
			return;
		}

		internalOn(led, level);

		prevStates[led] = level;

		if(currentFunctions.contains(led)){
			currentFunctions.erase(led);
		}
		if(prevFunctions.contains(led)){
			prevFunctions.erase(led);
		}
	}

	void set(LED led, std::unique_ptr<LEDFunction<LED, DataT>> func, bool temp = false){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(LogCMF, Error, "LED %d not registered", (int) led);
			return;
		}

		RegisteredFunction regFunc = { std::move(func), temp };

		if(currentFunctions.contains(led)){
			if(!currentFunctions[led].temp){
				prevFunctions[led] = currentFunctions[led];
			}
		}

		currentFunctions[led] = regFunc;

	}

	DataT getValue(LED led) requires (std::same_as<DataT, float>){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(LogCMF, Error, "LED %d not registered", (int) led);
			return DataT();
		}

		const auto& pin = outputs[led][0];
		return pin.driver->getState(pin.port);
	}

	DataT getValue(LED led) requires (std::same_as<DataT, glm::vec3>){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(LogCMF, Error, "LED %d not registered", (int) led);
			return DataT();
		}

		DataT level;
		for(int i = 0; i < outputs[led].size(); i++){
			level[i] = outputs[led][i].driver->getState(outputs[led][i].port);
		}
		return level;
	}

	void tick(float deltaTime) noexcept override{
		SyncEntity::tick(deltaTime);

		std::lock_guard guard(accessMutex);

		for(auto& [led, func]: currentFunctions){
			DataT level = func.function->loop(deltaTime);
			internalOn(led, level);

			if(!func.function->isDone()) continue;

			if(prevFunctions.contains(led)){
				currentFunctions[led] = prevFunctions[led];
				prevFunctions.erase(led);
			}else if(prevStates.contains(led)){
				internalOn(led, prevStates[led]);
			}else{
				internalOff(led);
			}
		}
	}

private:
	struct RegisteredFunction {
		std::shared_ptr<LEDFunction<LED, DataT>> function;
		bool temp;
	};

	std::map<LED, std::array<OutputPin, sizeof(DataT) / sizeof(float)>> outputs;
	std::map<LED, RegisteredFunction> currentFunctions;
	std::map<LED, RegisteredFunction> prevFunctions;
	std::map<LED, DataT> prevStates;

	std::mutex accessMutex;

	void internalOn(LED led, DataT level) requires (std::same_as<DataT, float>){
		const auto& pin = outputs[led][0];
		pin.driver->write(pin.port, level);
	}

	void internalOn(LED led, DataT level) requires (std::same_as<DataT, glm::vec3>){
		for(int i = 0; i < outputs[led].size(); i++){
			outputs[led][i].driver->write(outputs[led][i].port, level[i]);
		}
	}

	void internalOff(LED led){
		for(const OutputPin& pin: outputs[led]){
			pin.driver->write(pin.port, false);
		}
	}
};

template<typename Monos, typename RGBs>
class LED : public AsyncEntity {
	GENERATED_BODY(LED, AsyncEntity)

public:
	LED(){
		monos = newObject<LEDBase<Monos, float>>(this);
		rgbs = newObject<LEDBase<RGBs, glm::vec3>>(this);
	}

	void reg(Monos led, OutputPin pin){
		monos->reg(led, { pin });
	}

	void reg(RGBs led, OutputPin pin_r, OutputPin pin_g, OutputPin pin_b){
		rgbs->reg(led, { pin_r, pin_g, pin_b });
	}

	void off(Monos led){
		monos->off(led);
	}

	void off(RGBs led){
		rgbs->off(led);
	}

	void on(Monos led, float level){
		monos->on(led, level);
	}

	void on(RGBs led, glm::vec3 level){
		rgbs->on(led, level);
	}

	void set(Monos led, std::unique_ptr<LEDFunction<Monos, float>> func, bool temp = false){
		monos->set(led, std::move(func), temp);
	}

	void set(RGBs led, std::unique_ptr<LEDFunction<RGBs, glm::vec3>> func, bool temp = false){
		rgbs->set(led, std::move(func), temp);
	}

	float getValue(Monos led){
		return monos->getValue(led);
	}

	glm::vec3 getValue(RGBs led){
		return rgbs->getValue(led);
	}

private:
	StrongObjectPtr<LEDBase<Monos, float>> monos;
	StrongObjectPtr<LEDBase<RGBs, glm::vec3>> rgbs;
};

template<typename LED, typename DataT>
class LEDFunction {
	friend class LEDBase<LED, DataT>;

public:
	virtual ~LEDFunction() = default;

private:
	virtual DataT loop(float dt) = 0;

	virtual bool isDone() = 0;
};

#endif //CMF_LED_H
