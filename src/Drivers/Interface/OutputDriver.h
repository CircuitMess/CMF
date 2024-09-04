#ifndef CMF_OUTPUTDRIVER_H
#define CMF_OUTPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"

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
	class OutputDriver* driver;
	int port;
};

class OutputDriver : public SyncEntity {
	GENERATED_BODY(OutputDriver, SyncEntity);

public:
	virtual float getState(int port) const noexcept;
	virtual void write(int port, float value) noexcept;
	virtual void write(int port, bool value) noexcept;
	virtual void send() noexcept;

	void removeOutput(int port);

	void removeOutput(OutputPinDef output);

protected:
	OutputDriver() = default;
	OutputDriver(const std::vector<OutputPinDef>& outputs);


	std::vector<OutputPinDef>& getOutputs();
	std::map<int, float>& getStates();
	std::map<int, bool>& getInversions();

private:
	void postInitProperties() noexcept override final;

	virtual void performRegister(OutputPinDef output);

	virtual void performDeregister(OutputPinDef output);

	std::vector<OutputPinDef> outputs;

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
