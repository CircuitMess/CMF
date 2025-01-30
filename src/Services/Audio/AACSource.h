#ifndef CMF_TEMPLATE_AACSOURCE_H
#define CMF_TEMPLATE_AACSOURCE_H

#include "AudioSource.h"
#include <aacdec.h>


class AACSource : public AudioSource {
	GENERATED_BODY(AACSource, AudioSource)

	using SampleType = int16_t;

public:
	AACSource() = default;

	void open(std::string path) override;

	void close() override;

	size_t getData(uint8_t* buffer, size_t bytes) override;

private:
	HAACDecoder decoder = nullptr;

	std::ifstream file;
	int bytesRemaining = 0;

	std::vector<char> fillBuffer;
	std::vector<char> dataBuffer;

	static constexpr size_t FileReadThreshold = 1024;
	static constexpr size_t FileReadChunkSize = 1024;
	static constexpr size_t ChannelNumber = 1;
	static constexpr size_t SampleSize = sizeof(SampleType);
	static constexpr size_t DecodeOutBufferSize = ChannelNumber * 1024 * SampleSize; // Size of decoded data from one decode pass is 1024 samples per channel

};


#endif //CMF_TEMPLATE_AACSOURCE_H
