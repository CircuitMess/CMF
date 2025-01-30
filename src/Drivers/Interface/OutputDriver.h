#ifndef CMF_OUTPUTDRIVER_H
#define CMF_OUTPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"
#include "Log/Log.h"

struct OutputPinDef {
	int port;
	bool inverted;

	bool operator==(const OutputPinDef& rhs) const noexcept{
		return port == rhs.port && inverted == rhs.inverted;
	}

	bool operator!=(const OutputPinDef& rhs) const noexcept{
		return !(rhs == *this);
	}
};

class OutputDriverBase : public Object {
	GENERATED_BODY(OutputDriverBase, Object)

public:
	virtual float getState(int port) const noexcept{ return 0.0f; }

	virtual void write(int port, float value) noexcept{ }

	void write(int port, bool value) noexcept{
		write(port, value ? 1.0f : 0.0f);
	}

	virtual void send() noexcept{ }

	virtual void performWrite(int port, float value) noexcept{ }
};

struct OutputPin {
	OutputDriverBase* driver;
	int port;
};

template<typename T = OutputPinDef> requires std::derived_from<T, OutputPinDef>
class OutputDriver : public OutputDriverBase {
	TEMPLATE_ATTRIBUTES(T)
	GENERATED_BODY(OutputDriver, OutputDriverBase)

public:
	virtual float getState(int port) const noexcept override{
		if(!states.contains(port)){
			return 0;
		}
		return states.at(port);
	}

	virtual void write(int port, float value) noexcept override final{
		states[port] = value;
		performWrite(port, getInversions()[port] ? 1.0 - value : value);
	}

	void registerOutput(T pinDef) noexcept{
		outputs.emplace_back(pinDef);
		inversions[pinDef.port] = pinDef.inverted;
		performRegister(pinDef);
	}

	void removeOutput(int port) noexcept{
		auto it = std::remove_if(outputs.begin(), outputs.end(), [port](const T& pinDef){
			return pinDef.port == port;
		});
		for(auto i = it; i != outputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
			inversions.erase(i->port);
		}
		outputs.erase(it, outputs.end());
	}

	void removeOutput(T output) noexcept{
		auto it = std::remove(outputs.begin(), outputs.end(), output);
		for(auto i = it; i != outputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
			inversions.erase(i->port);
		}
		outputs.erase(it, outputs.end());
	}

protected:
	OutputDriver() noexcept = default;

	OutputDriver(const std::vector<T>& outputs) noexcept : outputs(outputs){

	}

	std::vector<T>& getOutputs() noexcept{
		return outputs;
	}

	std::map<int, float>& getStates() noexcept{
		return states;
	}

	std::map<int, bool>& getInversions() noexcept{
		return inversions;
	}

private:
	void postInitProperties() noexcept override final{
		Super::postInitProperties();

		for(const auto& output: outputs){
			inversions[output.port] = output.inverted;
			performRegister(output);
		}
	}

	virtual void performRegister(T output) noexcept{ }

	virtual void performDeregister(T output) noexcept{ }

	std::vector<T> outputs;

	/**
	 * Map of cached output values.
	 * key = port[int] , value = state[float]
	 */
	std::map<int, float> states;

	/**
	 * Map of inversion settings for each port.
	 * key = port[int], value = inversion[bool]
	 */
	std::map<int, bool> inversions;

};

#endif //CMF_OUTPUTDRIVER_H
