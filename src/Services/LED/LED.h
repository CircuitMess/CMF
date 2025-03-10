#ifndef CMF_LED_H
#define CMF_LED_H

#include "Entity/AsyncEntity.h"
#include "Misc/Enum.h"
#include "glm.hpp"
#include "Drivers/Interface/OutputDriver.h"
#include "Log/Log.h"
#include "Memory/ObjectMemory.h"
#include "Object/SubclassOf.h"

DEFINE_LOG(LED)

template<typename LED, typename DataT>
class LEDFunction : public SyncEntity {
	TEMPLATE_ATTRIBUTES(LED, DataT)
	GENERATED_BODY(LEDFunction, SyncEntity)

public:
	virtual bool isDone() const noexcept{ return true; }

	virtual DataT getValue() const noexcept{ return {}; }
};

template<typename LED, typename DataT> requires (std::same_as<DataT, glm::vec3> || std::same_as<DataT, float>)
class LEDBase : public SyncEntity {
	TEMPLATE_ATTRIBUTES(LED, DataT)
	GENERATED_BODY(LEDBase, SyncEntity)

public:
	void reg(LED led, std::array<OutputPin, sizeof(DataT) / sizeof(float)> pins) noexcept{
		accessMutex.lock();
		outputs[led] = pins;
		accessMutex.unlock();
		off(led);
	}

	void off(LED led) noexcept{
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(CMF, Error, "LED %d not registered", (int) led);
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

	void on(LED led, DataT level) noexcept{
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(CMF, Error, "LED %d not registered", (int) led);
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

	void set(LED led, StrongObjectPtr<LEDFunction<LED, DataT>> function, bool temp = false) noexcept{
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(CMF, Error, "LED %d not registered", (int) led);
			return;
		}

		function->setOwner(this);

		RegisteredFunction regFunc = { std::move(function), temp };

		if(currentFunctions.contains(led)){
			if(!currentFunctions[led].isTemporary){
				prevFunctions[led] = std::move(currentFunctions[led]);
			}
		}

		currentFunctions[led] = std::move(regFunc);
	}

	DataT getValue(LED led) noexcept requires (std::same_as<DataT, float>){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(CMF, Error, "LED %d not registered", (int) led);
			return DataT();
		}

		const auto& pin = outputs[led][0];
		return pin.driver->getState(pin.port);
	}

	DataT getValue(LED led) noexcept requires (std::same_as<DataT, glm::vec3>){
		std::lock_guard guard(accessMutex);
		if(!outputs.contains(led)){
			CMF_LOG(CMF, Error, "LED %d not registered", (int) led);
			return DataT();
		}

		DataT level;
		for(int i = 0; i < outputs[led].size(); i++){
			level[i] = outputs[led][i].driver->getState(outputs[led][i].port);
		}
		return level;
	}

	void tick(float deltaTime) noexcept override{
		Super::tick(deltaTime);

		std::lock_guard guard(accessMutex);

		if(currentFunctions.empty()) return;

		for(auto it = currentFunctions.begin(); it != currentFunctions.end();){
			const auto& func = it->second;
			const auto& led = it->first;

			if(!func.function->isDone()){
				const DataT level = func.function->getValue();
				internalOn(led, level);

				++it;
				continue;
			}

			if(prevFunctions.contains(led)){
				currentFunctions[led].function = std::move(prevFunctions[led].function);
				currentFunctions[led].isTemporary = prevFunctions[led].isTemporary;
				prevFunctions.erase(led);
				++it;
			}else if(prevStates.contains(led)){
				internalOn(led, prevStates[led]);
				it = currentFunctions.erase(it);
			}else{
				internalOff(led);
				it = currentFunctions.erase(it);
			}
		}
	}

private:
	void internalOn(LED led, DataT level) noexcept requires (std::same_as<DataT, float>){
		const auto& pin = outputs[led][0];

		if(!pin.driver){
			CMF_LOG(CMF, Error, "LED %d driver not set", (int) led);
			return;
		}

		pin.driver->write(pin.port, level);
	}

	void internalOn(LED led, DataT level) noexcept requires (std::same_as<DataT, glm::vec3>){
		for(int i = 0; i < outputs[led].size(); i++){

			if(!outputs[led][i].driver) continue;

			outputs[led][i].driver->write(outputs[led][i].port, level[i]);
		}
	}

	void internalOff(LED led) noexcept{
		for(const OutputPin& pin: outputs[led]){

			if(!pin.driver) continue;

			pin.driver->write(pin.port, false);
		}
	}

	struct RegisteredFunction {
		StrongObjectPtr<LEDFunction<LED, DataT>> function;
		bool isTemporary;
	};

	std::map<LED, std::array<OutputPin, sizeof(DataT) / sizeof(float)>> outputs;
	std::map<LED, RegisteredFunction> currentFunctions;
	std::map<LED, RegisteredFunction> prevFunctions;
	std::map<LED, DataT> prevStates;

	std::mutex accessMutex;
};

template<typename Monos, typename RGBs>
class LED : public AsyncEntity {
	TEMPLATE_ATTRIBUTES(Monos, RGBs)
	GENERATED_BODY(LED, AsyncEntity)

public:
	LED() noexcept : Super(10, 4 * 1024, 6, 1) {
		monos = newObject<LEDBase<Monos, float>>(this);
		rgbs = newObject<LEDBase<RGBs, glm::vec3>>(this);
	}

	void reg(Monos led, OutputPin pin) noexcept{
		monos->reg(led, { pin });
	}

	void reg(const std::vector<std::pair<Monos, OutputPin>>& monoRegs){
		for(const auto& entry: monoRegs){
			reg(entry.first, entry.second);
		}
	}

	void reg(RGBs led, OutputPin pin_r, OutputPin pin_g, OutputPin pin_b) noexcept{
		rgbs->reg(led, { pin_r, pin_g, pin_b });
	}

	void reg(const std::vector<std::tuple<RGBs, OutputPin, OutputPin, OutputPin>>& rgbRegs){
		for(const auto& entry: rgbRegs){
			reg(std::get<0>(entry), std::get<1>(entry), std::get<2>(entry), std::get<3>(entry));
		}
	}

	void off(Monos led) noexcept{
		monos->off(led);
	}

	void off(RGBs led) noexcept{
		rgbs->off(led);
	}

	void on(Monos led, float level) noexcept{
		monos->on(led, level);
	}

	void on(RGBs led, glm::vec3 level) noexcept{
		rgbs->on(led, level);
	}

	void set(Monos led, StrongObjectPtr<LEDFunction<Monos, float>> function, bool temp = false) noexcept{
		monos->set(led, std::move(function), temp);
	}

	void set(RGBs led, StrongObjectPtr<LEDFunction<RGBs, glm::vec3>> function, bool temp = false) noexcept{
		rgbs->set(led, std::move(function), temp);
	}

	float getValue(Monos led) noexcept{
		return monos->getValue(led);
	}

	glm::vec3 getValue(RGBs led) noexcept{
		return rgbs->getValue(led);
	}

private:
	StrongObjectPtr<LEDBase<Monos, float>> monos;
	StrongObjectPtr<LEDBase<RGBs, glm::vec3>> rgbs;
};

#endif //CMF_LED_H
