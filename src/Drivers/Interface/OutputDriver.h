#ifndef CMF_OUTPUTDRIVER_H
#define CMF_OUTPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"
#include "Log/Log.h"

struct OutputPinDef {
	int port;
	bool inverted;

	bool operator==(const OutputPinDef& rhs) const{
		return port == rhs.port && inverted == rhs.inverted;
	}

	bool operator!=(const OutputPinDef& rhs) const{
		return !(rhs == *this);
	}

};

struct OutputPin {
	Object* driver;
	int port;
};

template<typename T = OutputPinDef> requires std::derived_from<T, OutputPinDef>
class OutputDriver : public SyncEntity {
	GENERATED_BODY(OutputDriver, SyncEntity);

public:
	virtual float getState(int port) const noexcept{
		if(!states.contains(port)){
			CMF_LOG(CMF, Error, "Output port %d not registered", port);
			return 0;
		}
		return states.at(port);
	}

	void write(int port, float value) noexcept{
		states[port] = value;
		performWrite(port, value);
	}

	void write(int port, bool value) noexcept{
		write(port, value ? 1.f : 0.f);
	}

	virtual void send() noexcept{}

	void removeOutput(int port){
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

	void removeOutput(T output){
		auto it = std::remove(outputs.begin(), outputs.end(), output);
		for(auto i = it; i != outputs.end(); ++i){
			performDeregister(*i);
			states.erase(i->port);
			inversions.erase(i->port);
		}
		outputs.erase(it, outputs.end());
	}

protected:
	OutputDriver() = default;

	OutputDriver(const std::vector<T>& outputs) : outputs(outputs){

	}

	std::vector<T>& getOutputs(){
		return outputs;
	}

	std::map<int, float>& getStates(){
		return states;
	}

	std::map<int, bool>& getInversions(){
		return inversions;
	}

private:
	void postInitProperties() noexcept override final{
		SyncEntity::postInitProperties();

		for(const auto& output: outputs){
			performRegister(output);
		}
	}

	virtual void performRegister(T output){}

	virtual void performDeregister(T output){}

	virtual void performWrite(int port, float value){}

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
