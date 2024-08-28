#ifndef CMF_INPUTDRIVER_H
#define CMF_INPUTDRIVER_H


#include "Misc/Enum.h"
#include "Entity/SyncEntity.h"

enum class PullMode : uint8_t {
	None, Up, Down
};

struct InputPinDef {
	int port;
	bool inverted;
	PullMode pullMode;
};

struct InputPin {
	class InputDriver* driver;

	int port;
};

class InputDriver : public SyncEntity {
	GENERATED_BODY(InputDriver, SyncEntity);

public:
	virtual void scan() noexcept;
	virtual float read(int port) noexcept;

protected:
	InputDriver() = default;
	InputDriver(const std::vector<InputPinDef>& inputs);

	std::vector<InputPinDef> inputs;

	/**
	 * Map of cached input values.
	 * key = port[int] , value = state[bool]
	 */
	std::map<int, bool> states;
};

#endif //CMF_INPUTDRIVER_H
