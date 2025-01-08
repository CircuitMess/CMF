#include "I2C.h"
#include <driver/i2c.h>
#include "Log/Log.h"

I2C::I2C(I2CPort port, gpio_num_t sda, gpio_num_t scl) noexcept : Super(), port(i2c_port_t(port)){
	if(port == I2CPort::None){
		return;
	}

	if(sda == GPIO_NUM_NC || scl == GPIO_NUM_NC){
		return;
	}

	const i2c_config_t config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = sda,
		.scl_io_num = scl,
		.sda_pullup_en = false,
		.scl_pullup_en = false,
		.master = {.clk_speed = 400000}, // TODO: maybe make this configurable in the future
		.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL
	};

	ESP_ERROR_CHECK(i2c_driver_install(this->port, I2C_MODE_MASTER, 0, 0, 0));
	ESP_ERROR_CHECK(i2c_param_config(this->port, &config));
}

I2C::~I2C() noexcept{
	ESP_ERROR_CHECK(i2c_driver_delete(port));
}

I2CPort I2C::getPort() const noexcept{
	return (I2CPort) port;
}

void I2C::scan(TickType_t timeout) noexcept{
	CMF_LOG(CMF, Verbose, "I2C scan:");

	for(size_t i = 0; i < 127; ++i){
		if(probe(i, timeout) == ESP_OK){
			CMF_LOG(CMF, Verbose, "Found device on address 0x%x", i);
		}
	}

	CMF_LOG(CMF, Verbose, "I2C scan done.");
}

esp_err_t I2C::probe(uint8_t address, TickType_t timeout) noexcept{
	std::lock_guard lock(mutex);

	auto cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (address << 1) | I2C_MASTER_WRITE, true);
	i2c_master_stop(cmd);

	auto status = i2c_master_cmd_begin(port, cmd, timeout);
	i2c_cmd_link_delete(cmd);
	return status;
}

esp_err_t I2C::write(uint8_t address, const std::vector<uint8_t>& data, TickType_t wait){
	std::lock_guard lock(mutex);
	return i2c_master_write_to_device(port, address, data.data(), data.size(), wait);
}

esp_err_t I2C::write(uint8_t address, uint8_t data, TickType_t wait){
	return write(address, std::vector<uint8_t>({ data }), wait);
}

esp_err_t I2C::write(uint8_t address, std::initializer_list<uint8_t> data, TickType_t wait){
	return write(address, std::vector<uint8_t>({ data }), wait);
}

esp_err_t I2C::read(uint8_t address, std::vector<uint8_t>& data, TickType_t wait){
	std::lock_guard lock(mutex);
	return i2c_master_read_from_device(port, address, data.data(), data.size(), wait);
}

esp_err_t I2C::read(uint8_t address, uint8_t& data, TickType_t wait){
	std::lock_guard lock(mutex);
	return i2c_master_read_from_device(port, address, &data, 1, wait);
}

esp_err_t I2C::write_read(uint8_t address, const std::vector<uint8_t>& writeData, std::vector<uint8_t>& readData, TickType_t wait){
	std::lock_guard lock(mutex);
	return i2c_master_write_read_device(port, address, writeData.data(), writeData.size(), readData.data(), readData.size(), wait);
}

esp_err_t I2C::write_read(uint8_t address, uint8_t writeData, std::vector<uint8_t>& readData, TickType_t wait){
	return write_read(address, std::vector<uint8_t>({ writeData }), readData, wait);
}

esp_err_t I2C::write_read(uint8_t address, const std::vector<uint8_t>& writeData, uint8_t& readData, TickType_t wait){
	std::lock_guard lock(mutex);
	return i2c_master_write_read_device(port, address, writeData.data(), writeData.size(), &readData, 1, wait);
}

esp_err_t I2C::write_read(uint8_t address, uint8_t writeData, uint8_t& readData, TickType_t wait){
	return write_read(address, std::vector<uint8_t>({ writeData }), readData, wait);
}

esp_err_t I2C::writeRegister(uint8_t address, uint8_t reg, const std::vector<uint8_t>& data, TickType_t wait){
	std::vector<uint8_t> buffer(data.size() + 1);
	buffer[0] = reg;

	memcpy(buffer.data() + 1, data.data(), data.size());

	return write(address, buffer, wait);
}

esp_err_t I2C::writeRegister(uint8_t address, uint8_t reg, const uint8_t* data, size_t size, TickType_t wait){
	std::vector<uint8_t> buf(size+1);
	buf[0] = reg;
	memcpy(buf.data()+1, data, size);
	return write(address, buf, wait);
}

esp_err_t I2C::writeRegister(uint8_t address, uint8_t reg, uint8_t data, TickType_t wait){
	std::vector<uint8_t> buffer(2);
	buffer[0] = reg;
	buffer[1] = data;

	return write(address, buffer, wait);
}

esp_err_t I2C::writeRegister(uint8_t address, uint8_t reg, std::initializer_list<uint8_t> data, TickType_t wait){
	std::vector<uint8_t> buf(data);
	return writeRegister(address, reg, buf, wait);
}

esp_err_t I2C::readRegister(uint8_t address, uint8_t reg, std::vector<uint8_t>& data, TickType_t wait){
	return write_read(address, reg, data, wait);
}

esp_err_t I2C::readRegister(uint8_t address, uint8_t reg, uint8_t& data, TickType_t wait){
	return write_read(address, reg, data, wait);
}

std::lock_guard<std::mutex> I2C::lockBus() noexcept{
	return std::lock_guard<std::mutex>(mutex);
}
