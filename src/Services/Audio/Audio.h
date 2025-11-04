#ifndef CMF_AUDIO_H
#define CMF_AUDIO_H

#include <optional>
#include "Object/Class.h"
#include "Entity/AsyncEntity.h"
#include "Drivers/Interface/OutputDriver.h"
#include "AudioSource.h"
#include "Periphery/I2S.h"

class Audio : public AsyncEntity {
	GENERATED_BODY(Audio, AsyncEntity, CONSTRUCTOR_PACK(StrongObjectPtr<I2S>, Object*, OutputPin))

public:
	Audio() = default;

	Audio(StrongObjectPtr<I2S> i2s, Object* source, OutputPin enablePin);

	Audio(StrongObjectPtr<I2S> i2s, Object* source);

	~Audio() override;

	void play(const std::string& path);

	void stop();

	void setEnabled(bool enabled);

	void setGain(float gain);


protected:
	void tick(float deltaTime) noexcept override;

private:
	StrongObjectPtr<I2S> i2s;
	std::optional<OutputPin> enablePin = std::nullopt;
	StrongObjectPtr<AudioSource> source;

	bool enabled = true;

	static constexpr size_t BufSize = 1024;
	std::array<int16_t, BufSize> dataBuf;

	bool playing = false;

	SemaphoreHandle_t playSemaphore;

	float gain = 1.0f;
};

#endif //CMF_AUDIO_H