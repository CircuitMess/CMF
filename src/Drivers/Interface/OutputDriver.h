#ifndef CMF_OUTPUTDRIVER_H
#define CMF_OUTPUTDRIVER_H

#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"

struct OutputPinDef {
	int port;
	bool inverted;
};

struct OutputPin {
	class OutputDriver* driver;

	int port;
};

class OutputDriver : public SyncEntity {
	GENERATED_BODY(OutputDriver, SyncEntity);

public:
	virtual float getState(int port) noexcept;
	virtual void write(int port, float value) noexcept;
	virtual void write(int port, bool value) noexcept;
	virtual void send() noexcept;

protected:
	OutputDriver() = default;
	OutputDriver(const std::vector<OutputPinDef>& outputs);

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
