#include "RM_TempHumModule.h"

RM_TempHumModule::RM_TempHumModule(const Modules::BusPins& busPins) : ModuleDevice(Type::RM_TempHum, busPins){
	ESP_ERROR_CHECK(busPins.i2c->write(Addr, 0x00));
}

void RM_TempHumModule::sample(){
	const auto data = readData();
	temp = (int16_t) getTemp(data);
	hum = (uint16_t) getHumidity(data);
}

int16_t RM_TempHumModule::getTemperature() const{
	return temp;
}

uint8_t RM_TempHumModule::getHumidity() const{
	return hum;
}

//TODO - separate this out to its own Device class for AHT20
std::array<uint8_t, 6> RM_TempHumModule::readData(){
	std::array<uint8_t, 6> data{};
	pins.i2c->write(Addr, { 0xAC, 0x33, 0x00 });
	delayMillis(80);
	pins.i2c->read(Addr, data);

	return data;
}

float RM_TempHumModule::getHumidity(const std::array<uint8_t, 6>& data){
	const int h = data[1] << 12 | data[2] << 4 | data[3] >> 4;
	const float hum = ((float) h * 100.0f) / 0x100000;

	return hum;
}

float RM_TempHumModule::getTemp(const std::array<uint8_t, 6>& data){
	const int t = (data[3] & 0x0F) << 16 | data[4] << 8 | data[5];
	const float temp = ((float) t * 200.0f / 0x100000) - 50;

	return temp;
}