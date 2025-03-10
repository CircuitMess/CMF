#include "Audio.h"
#include <utility>

DEFINE_LOG(Audio)

Audio::Audio(StrongObjectPtr<I2S> i2s, Object* source, OutputPin enablePin) : Audio(std::move(i2s), source){
	this->enablePin = enablePin;
}

Audio::Audio(StrongObjectPtr<I2S> i2s, Object* source) : AsyncEntity(0, 4 * 1024, 7, 1), i2s(std::move(i2s)){
	if(!source->isA<AudioSource>()){
		CMF_LOG(Audio, LogLevel::Error, "Given source isn't an AudioSource instance!");
		abort();
	}

	playSemaphore = xSemaphoreCreateBinary();
	xSemaphoreTake(playSemaphore, 0);

	this->source = cast<AudioSource>(source);

	if(enablePin){
		enablePin->driver->write(enablePin->port, true);
	}
}

Audio::~Audio(){
	AsyncEntity::onDestroy();
	stop();

	if(enablePin){
		enablePin->driver->write(enablePin->port, false);
	}
}

void Audio::play(const std::string& path){
	if(!enabled) return;

	if(playing){
		stop();
	}

	if(enablePin){
		enablePin->driver->write(enablePin->port, true);
	}

	source->open(path);

	playing = true;

	xSemaphoreGive(playSemaphore);
}

void Audio::stop(){
	if(enablePin){
		enablePin->driver->write(enablePin->port, false);
	}
	source->close();
	playing = false;
}

void Audio::tick(float deltaTime) noexcept{
	if(!xSemaphoreTake(playSemaphore, portMAX_DELAY)) {
		return;
	}

	if(!playing) {
		return;
	}

	const size_t bytesToTransfer = source->getData((uint8_t*)dataBuf.data(), BufSize * sizeof(int16_t));

	CMF_LOG(Audio, LogLevel::Debug, "audio tick bytes: %d", bytesToTransfer);

	if(bytesToTransfer == 0){
		stop();
		return;
	}

	if(gain != 1.0f){
		for(auto& sample: dataBuf){
			sample = (int16_t) (sample * gain);
		}
	}

	i2s->write(reinterpret_cast<uint8_t *>(dataBuf.data()), bytesToTransfer);
}

void Audio::setEnabled(bool enabled){
	Audio::enabled = enabled;
	if(enabled) return;

	stop();
}

void Audio::setGain(float gain){
	this->gain = gain;
}