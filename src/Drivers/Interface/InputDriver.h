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

	bool operator==(const InputPinDef& rhs) const{
		return port == rhs.port && inverted == rhs.inverted && pullMode == rhs.pullMode;
	}

	bool operator!=(const InputPinDef& rhs) const{
		return !(rhs == *this);
	}
};

struct InputPin {
	class InputDriver* driver;
	int port;
};

class InputDriver : public SyncEntity {
	GENERATED_BODY(InputDriver, SyncEntity);

public:
	virtual void scan() noexcept;
	virtual float read(int port) const noexcept;

	void registerInput(InputPinDef pinDef);

	void removeInput(int port);

	void removeInput(InputPinDef input);

	void begin() noexcept override;

protected:
	InputDriver() = default;
	InputDriver(const std::vector<InputPinDef>& inputs);

	void forEachInput(std::function<void(const InputPinDef&)> func) const;
	std::vector<InputPinDef>& getInputs();
	std::map<int, bool>& getStates();

private:
	void postInitProperties() noexcept override final;

	virtual void performRegister(InputPinDef input);
	virtual void performDeregister(InputPinDef input);

	std::vector<InputPinDef> inputs;

	/**
	 * Map of cached input values.
	 * key = port[int] , value = state[bool]
	 */
	std::map<int, bool> states;
};

#endif //CMF_INPUTDRIVER_H
