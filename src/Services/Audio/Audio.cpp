#include "Audio.h"
#include <utility>

DEFINE_LOG(Audio)

Audio::Audio(StrongObjectPtr<I2S> i2s, OutputPin enablePin) : Audio(std::move(i2s)){
	this->enablePin = enablePin;
}

Audio::Audio(StrongObjectPtr<I2S> i2s) : AsyncEntity(0, 4 * 1024, 7, -1), i2s(std::move(i2s)){
	playSemaphore = xSemaphoreCreateBinary();
	xSemaphoreTake(playSemaphore, 0);

	if(enablePin){
		enablePin->driver->write(enablePin->port, true);
	}
}

Audio::~Audio(){
	stop();

	if(enablePin){
		enablePin->driver->write(enablePin->port, false);
	}
}

void Audio::play(AudioGenerator* generator, AudioSource* source){
	if(!enabled) return;

	if(playing){
		stop();
	}

	if(enablePin){
		enablePin->driver->write(enablePin->port, true);
	}

	generator->open(source);
	this->generator = generator;

	playing = true;

	xSemaphoreGive(playSemaphore);
}

void Audio::stop() {
	if(!enabled || !playing) {
		return;
	}

	if(!xSemaphoreTake(playSemaphore, portMAX_DELAY)) {
		return;
	}

	if(enablePin){
		enablePin->driver->write(enablePin->port, false);
	}
	generator->close();
	playing = false;
}

void Audio::tick(float deltaTime) noexcept{
	Super::tick(deltaTime);

	if(!xSemaphoreTake(playSemaphore, portMAX_DELAY)) {
		return;
	}

	if(!playing) {
		return;
	}

	const size_t bytesToTransfer = generator->getData((uint8_t*)dataBuf.data(), BufSize * sizeof(int16_t));

	CMF_LOG(Audio, LogLevel::Debug, "audio tick bytes: %d", bytesToTransfer);

	if(bytesToTransfer == 0){
		xSemaphoreGive(playSemaphore);
		stop();
		return;
	}

	if(gain != 1.0f){
		for(auto& sample: dataBuf){
			sample = (int16_t) (sample * gain);
		}
	}

	i2s->write(reinterpret_cast<uint8_t *>(dataBuf.data()), bytesToTransfer);

	xSemaphoreGive(playSemaphore);
}

void Audio::setEnabled(bool enabled){
	Audio::enabled = enabled;
	if(enabled) return;

	stop();
}

void Audio::setGain(float gain){
	this->gain = gain;
}