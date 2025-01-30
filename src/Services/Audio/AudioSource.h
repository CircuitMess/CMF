#ifndef CMF_AUDIOSOURCE_H
#define CMF_AUDIOSOURCE_H

#include <fstream>
#include "Log/Log.h"
#include "Object/Object.h"

DEFINE_LOG(AudioSource)

class AudioSource : public Object {
	GENERATED_BODY(AudioSource, Object)
public:

	virtual void open(const std::string& path){};

	virtual void close(){};

	/**
	 * Pull data from decoder.
	 * @param buffer output, to be filled with decoded data
	 * @param bytes number of requested bytes
	 * @return number of bytes actually written to provided buffer
	 */
	virtual size_t getData(uint8_t* buffer, size_t bytes){
		return 0;
	}
};

#endif //CMF_AUDIOSOURCE_H