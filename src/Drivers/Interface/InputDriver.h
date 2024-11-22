#ifndef CMF_INPUTDRIVER_H
#define CMF_INPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"
#include "Log/Log.h"

struct InputPinDef {
	int port;
	bool inverted;

	bool operator==(const InputPinDef& rhs) const{
		return port == rhs.port && inverted == rhs.inverted;
	}

	bool operator!=(const InputPinDef& rhs) const{
		return !(rhs == *this);
	}
};

struct InputPin {
	Object* driver;
	int port;
};

template<typename T = InputPinDef> requires std::derived_from<T, InputPinDef>
class InputDriver : public SyncEntity {
	GENERATED_BODY(InputDriver, SyncEntity);

public:
	virtual void scan() noexcept{};

	virtual float read(int port) const noexcept{
		if(!states.contains(port)){
			CMF_LOG(LogCMF, Error, "Input port %d not registered", port);
			return 0;
		}
		return states.at(port);
	}

	void registerInput(T pinDef){
		inputs.emplace_back(pinDef);
		performRegister(pinDef);
	}

	void removeInput(int port){
		auto it = std::remove_if(inputs.begin(), inputs.end(), [port](const InputPinDef& pinDef){
			return pinDef.port == port;
		});
		for(auto i = it; i != inputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
		}
		inputs.erase(it, inputs.end());
	}

	void removeInput(T input){
		auto it = std::remove(inputs.begin(), inputs.end(), input);
		for(auto i = it; i != inputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
		}
		inputs.erase(it, inputs.end());
	}


	void begin() noexcept override{
		Super::begin();

		scan();
	}

protected:
	InputDriver() = default;

	InputDriver(const std::vector<T>& inputs) : inputs(inputs){

	}

	void forEachInput(std::function<void(const T&)> func) const{
		for(const auto& input: inputs){
			func(input);
		}
	}

	std::vector<T>& getInputs(){
		return inputs;
	}

	std::map<int, bool>& getStates(){
		return states;
	}

private:
	void postInitProperties() noexcept override final{
		SyncEntity::postInitProperties();

		for(const auto& input: inputs){
			performRegister(input);
		}
	}

	virtual void performRegister(T input){}

	virtual void performDeregister(T input){}

	std::vector<T> inputs;

	/**
	 * Map of cached input values.
	 * key = port[int] , value = state[bool]
	 */
	std::map<int, bool> states;
};

#endif //CMF_INPUTDRIVER_H
