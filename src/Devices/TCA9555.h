#ifndef CMF_TCA9555_H
#define CMF_TCA9555_H

#include "Object/Object.h"

/**
 * GPIO expander, 16-bit.
 * https://www.ti.com/lit/ds/symlink/tca9555.pdf
 *
 * Known clone drop-in replacement: XINLUDA XL9555 (https://xinluda.com/en/I2C-to-GPIO-extension/20240718576.html)
 */
class TCA9555 : public Object {
	GENERATED_BODY(TCA9555, Object)

public:
	explicit TCA9555(class I2C* i2c = nullptr, uint8_t addr = 0x20);

	/**
	 * Reset all pins to input and clear register state.
	 */
	void reset();

	enum class PinMode : uint8_t {
		IN, OUT
	};

	/**
	 * Set pin mode
	 * @param pin Pin index
	 * @param mode Pin mode
	 */
	void pinMode(uint8_t pin, PinMode mode);

	/**
	 * Read pin input state.
	 * @param pin Pin index
	 * @return State - true for high, false for low
	 */
	bool read(uint8_t pin);

	/**
	 * Get read register for all pins.
	 * Does not check if all pins are Input-defined, so user is responsible for correct masking!
	 * @return bit-vector of pin states
	 */
	uint16_t readAll();

	/**
	 * Set pin output state.
	 * @param pin Pin index
	 * @param state True for high, false for low
	 */
	void write(uint8_t pin, bool state);

private:
	I2C* i2c;
	const uint8_t Addr;

	struct Regs {
		uint8_t dir[2] = { 0xff, 0xff };
		uint8_t output[2] = { 0x00, 0x00 };
	} regs;

	static constexpr uint8_t REG_INPUT = 0x00;
	static constexpr uint8_t REG_OUTPUT = 0x02;
	static constexpr uint8_t REG_POLARITY = 0x04;
	static constexpr uint8_t REG_DIR = 0x06;
};


#endif //CMF_TCA9555_H
