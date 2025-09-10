#ifndef CMF_I2C_H
#define CMF_I2C_H

#include "GPIOPeriph.h"
#include <hal/i2c_types.h>
#include <hal/gpio_types.h>
#include <esp_err.h>
#include <freertos/portmacro.h>
#include <driver/i2c.h>

enum class I2CPort : int8_t {
	None = -1,
	Zero = 0,
#if SOC_I2C_NUM >= 2
	One = 1,
#endif
#if SOC_LP_I2C_NUM >= 1
	LpZero,
#endif
	COUNT
};

class I2C : public Object {
	GENERATED_BODY(I2C, Object)

public:
	explicit I2C(I2CPort port = I2CPort::None, gpio_num_t sda = GPIO_NUM_NC, gpio_num_t scl = GPIO_NUM_NC) noexcept;
	virtual ~I2C() noexcept override;

	I2CPort getPort() const noexcept;

	void scan(TickType_t timeout = 5) noexcept;
	esp_err_t probe(uint8_t address, TickType_t timeout = 10) noexcept; // TODO: move away from esp errors and towards custom errors

	esp_err_t write(uint8_t address, const std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t write(uint8_t address, uint8_t data, TickType_t wait = portMAX_DELAY);
	esp_err_t write(uint8_t address, std::initializer_list<uint8_t> data, TickType_t wait = portMAX_DELAY);

	esp_err_t read(uint8_t address, std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);

	template<size_t Size>
	esp_err_t read(uint8_t address, std::array<uint8_t, Size>& data, TickType_t wait = portMAX_DELAY){
		std::lock_guard lock(mutex);
		return i2c_master_read_from_device(port, address, data.data(), Size, wait);
	}

	esp_err_t read(uint8_t address, uint8_t& data, TickType_t wait = portMAX_DELAY);

	esp_err_t write_read(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(uint8_t address, uint8_t writeData, std::vector<uint8_t>& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(uint8_t address, const std::vector<uint8_t>& writeData, uint8_t& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(uint8_t address, uint8_t writeData, uint8_t& readData, TickType_t wait = portMAX_DELAY);

	esp_err_t writeRegister(uint8_t address, uint8_t reg, const std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t writeRegister(uint8_t address, uint8_t reg, const uint8_t* data, size_t size, TickType_t wait = portMAX_DELAY);
	esp_err_t writeRegister(uint8_t address, uint8_t reg, uint8_t data, TickType_t wait = portMAX_DELAY);
	esp_err_t writeRegister(uint8_t address, uint8_t reg, std::initializer_list<uint8_t> data, TickType_t wait = portMAX_DELAY);

	esp_err_t readRegister(uint8_t address, uint8_t reg, std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t readRegister(uint8_t address, uint8_t reg, uint8_t& data, TickType_t wait = portMAX_DELAY);

	std::lock_guard<std::mutex> lockBus() noexcept;

private:
	const i2c_port_t port;
	std::mutex mutex;
};

#endif //CMF_I2C_H