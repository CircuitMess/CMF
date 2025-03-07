#ifndef CMF_INPUTDRIVER_H
#define CMF_INPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"
#include "Log/Log.h"

struct InputPinDef {
	int port;
	bool inverted;

	bool operator==(const InputPinDef& rhs) const noexcept{
		return port == rhs.port && inverted == rhs.inverted;
	}

	bool operator!=(const InputPinDef& rhs) const noexcept{
		return !(rhs == *this);
	}
};

class InputDriverBase : public Object {
	GENERATED_BODY(InputDriverBase, Object)

public:
	virtual void scan() noexcept{}

	virtual float read(int port) const noexcept{ return 0.0f; }
};

struct InputPin {
	InputDriverBase* driver;
	int port;
};

template<typename T = InputPinDef> requires std::derived_from<T, InputPinDef>
class InputDriver : public InputDriverBase {
	TEMPLATE_ATTRIBUTES(T);
	GENERATED_BODY(InputDriver, InputDriverBase);

public:
	virtual float read(int port) const noexcept override{
		if(!states.contains(port)){
			CMF_LOG(CMF, Error, "Input port %d not registered", port);
			return 0;
		}
		return states.at(port) ^ inversions.at(port);
	}

	void registerInput(T pinDef) noexcept{
		inputs.emplace_back(pinDef);
		inversions[pinDef.port] = pinDef.inverted;
		performRegister(pinDef);
	}

	void removeInput(int port) noexcept{
		auto it = std::remove_if(inputs.begin(), inputs.end(), [port](const InputPinDef& pinDef){
			return pinDef.port == port;
		});
		for(auto i = it; i != inputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
			inversions.erase(i->port);
		}
		inputs.erase(it, inputs.end());
	}

	void removeInput(T input) noexcept{
		auto it = std::remove(inputs.begin(), inputs.end(), input);
		for(auto i = it; i != inputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
			inversions.erase(i->port);
		}
		inputs.erase(it, inputs.end());
	}

protected:
	InputDriver() noexcept = default;

	InputDriver(const std::vector<T>& inputs) noexcept : inputs(inputs){

	}

	void forEachInput(std::function<void(const T&)> func) const noexcept{
		for(const auto& input: inputs){
			func(input);
		}
	}

	std::vector<T>& getInputs() noexcept{
		return inputs;
	}

	std::map<int, bool>& getStates() noexcept{
		return states;
	}

	std::map<int, bool>& getInversions() noexcept{
		return inversions;
	}

private:
	virtual void postInitProperties() noexcept override final{
		Super::postInitProperties();

		for(const auto& input: inputs){
			inversions[input.port] = input.inverted;
			performRegister(input);
		}
	}

	virtual void performRegister(T input) noexcept{ }

	virtual void performDeregister(T input) noexcept{ }

	std::vector<T> inputs;

	/**
	 * Map of cached input values.
	 * key = port[int] , value = state[bool]
	 */
	std::map<int, bool> states;

	/**
	 * Map of inversion settings for each port.
	 * key = port[int], value = inversion[bool]
	 */
	std::map<int, bool> inversions;
};

#endif //CMF_INPUTDRIVER_H
