#ifndef CMF_TEMPLATE_AUDIOSOURCE_H
#define CMF_TEMPLATE_AUDIOSOURCE_H

#include "Object/Object.h"
#include <fstream>
#include "Log/Log.h"

DEFINE_LOG(AudioSource)

class AudioSource : public Object {
	GENERATED_BODY(AudioSource, Object)
public:

	virtual void open(std::string path){

	};

	virtual void close(){

	};

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


#endif //CMF_TEMPLATE_AUDIOSOURCE_H
