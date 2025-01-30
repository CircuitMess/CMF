#ifndef CMF_TEMPLATE_AUDIO_H
#define CMF_TEMPLATE_AUDIO_H

#include "Entity/AsyncEntity.h"
#include "Drivers/Interface/OutputDriver.h"
#include "AudioSource.h"
#include <optional>
#include "Periphery/I2S.h"

class Audio : public AsyncEntity {
	GENERATED_BODY(Audio, AsyncEntity)

public:
	Audio() = default;

	Audio(StrongObjectPtr<I2S> i2s, Object* source, OutputPin enablePin);

	Audio(StrongObjectPtr<I2S> i2s, Object* source);

	~Audio() override;

	void play(std::string path);

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

	std::vector<int16_t> dataBuf;

	bool playing = false;

	static constexpr size_t BufSize = 1024;

	float gain = 1.0f;
};


#endif //CMF_TEMPLATE_AUDIO_H
