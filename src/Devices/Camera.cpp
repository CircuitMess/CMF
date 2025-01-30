#include "Camera.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

Camera::Camera(camera_config_t config, I2C* i2c, std::function<void(sensor_t*)> sensorConfig) : config(config), i2c(i2c), sensorConfig(sensorConfig){
	gpio = ApplicationStatics::getApplication()->getPeriphery<GPIOPeriph>();
	gpio->setMode((gpio_num_t) config.pin_pwdn, GPIOMode::Output);
	gpio->write((gpio_num_t) config.pin_pwdn, true);

	resWait = config.frame_size;
	formatWait = config.pixel_format;
}

Camera::~Camera(){
	deinit();
}

esp_err_t Camera::init(){
	if(resWait == res && formatWait == format && inited) return ESP_OK;

	if(inited){
		deinit();
	}

	format = formatWait;
	res = resWait;

	config.pin_pwdn = -1;

	config.frame_size = res;
	config.pixel_format = format;

	if(format == PIXFORMAT_JPEG){
		config.jpeg_quality = 12;
	}

	gpio->write((gpio_num_t) config.pin_pwdn, false);


	auto lock = i2c->lockBus();

	auto err = esp_camera_init(&config);
	if(err == ESP_ERR_NOT_FOUND){
		return err;
	}else if(err != ESP_OK){
		printf("Camera init failed with error 0x%x: %s\n", err, esp_err_to_name(err));
		return err;
	}

	sensor_t* sensor = esp_camera_sensor_get();
	if(sensor == nullptr){
		return ESP_ERR_CAMERA_NOT_DETECTED;
	}

	if(sensorConfig){
		sensorConfig(sensor);
	}

	inited = true;
	failedFrames = 0;

	return ESP_OK;
}

void Camera::deinit(){
	if(!inited) return;
	inited = false;

	if(frame){
		esp_camera_fb_return(frame);
		frame = nullptr;
	}

	{
		auto lock = i2c->lockBus();
		esp_camera_deinit();
	}

	gpio->write((gpio_num_t) config.pin_pwdn, true);
}

camera_fb_t* Camera::getFrame(){
	if(!inited) return nullptr;
	if(frame) return nullptr;

	frame = esp_camera_fb_get();

	if(frame == nullptr){
		failedFrames++;

		if(failedFrames >= MaxFailedFrames){
			deinit();
		}
	}else{
		failedFrames = 0;
	}

	return frame;
}

void Camera::releaseFrame(){
	if(!inited) return;
	if(!frame) return;

	esp_camera_fb_return(frame);
	frame = nullptr;
}

bool Camera::isInited(){
	return inited;
}

void Camera::setRes(framesize_t res){
	resWait = res;
}

framesize_t Camera::getRes() const{
	return res;
}

pixformat_t Camera::getFormat() const{
	if(format == PIXFORMAT_RGB565) return PIXFORMAT_RGB888;
	return format;
}

void Camera::setFormat(pixformat_t format){
	if(format == PIXFORMAT_RGB888){
		format = PIXFORMAT_RGB565;
	}

	formatWait = format;
}
