#ifndef CMF_FILEAUDIOSOURCE_H
#define CMF_FILEAUDIOSOURCE_H

#include "AudioSource.h"
#include "Object/Object.h"
#include <fstream>

class FileAudioSource : public Object, public AudioSource {
public:
	FileAudioSource(std::string path);

	void open() override;

	void close() override;

	size_t getData(uint8_t* buffer, size_t bytes) override;

	operator bool() const override;

private:
	const std::string path;
	std::ifstream file;
};


#endif //CMF_FILEAUDIOSOURCE_H
