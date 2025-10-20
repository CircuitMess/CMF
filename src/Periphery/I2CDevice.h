#ifndef CMF_I2C_DEVICE_H
#define CMF_I2C_DEVICE_H

#include <driver/i2c_types.h>
#include <freertos/FreeRTOS.h>
#include <vector>

class I2CDevice {
	friend class I2CMaster;

public:
	explicit I2CDevice(class I2CMaster* master, i2c_master_dev_handle_t hndl);
	virtual ~I2CDevice();

	esp_err_t write(const std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t write(uint8_t data, TickType_t wait = portMAX_DELAY);
	esp_err_t write(const uint8_t* data, size_t size, TickType_t wait = portMAX_DELAY);

	esp_err_t read(std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t read(uint8_t& data, TickType_t wait = portMAX_DELAY);
	esp_err_t read(uint8_t* data, size_t size, TickType_t wait = portMAX_DELAY);

	template<size_t Size>
	esp_err_t read(std::array<uint8_t, Size>& data, TickType_t wait = portMAX_DELAY){
		return read(data.data(), Size, wait);
	}

	esp_err_t write_read(const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(uint8_t writeData, std::vector<uint8_t>& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(const std::vector<uint8_t>& writeData, uint8_t& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(uint8_t writeData, uint8_t& readData, TickType_t wait = portMAX_DELAY);
	esp_err_t write_read(const uint8_t* writeData, size_t writeSize, uint8_t* readData, size_t readSize, TickType_t wait = portMAX_DELAY);

	esp_err_t writeRegister(uint8_t reg, const std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t writeRegister(uint8_t reg, uint8_t data, TickType_t wait = portMAX_DELAY);
	esp_err_t writeRegister(uint8_t reg, const uint8_t* data, size_t size, TickType_t wait = portMAX_DELAY);

	esp_err_t readRegister(uint8_t reg, std::vector<uint8_t>& data, TickType_t wait = portMAX_DELAY);
	esp_err_t readRegister(uint8_t reg, uint8_t& data, TickType_t wait = portMAX_DELAY);
	esp_err_t readRegister(uint8_t reg, uint8_t* data, size_t size, TickType_t wait = portMAX_DELAY);

private:
	I2CMaster* master;
	const i2c_master_dev_handle_t hndl;

};


#endif //CMF_I2C_DEVICE_H
