#ifndef CMF_RINGBUFFER_H
#define CMF_RINGBUFFER_H

#include <cstdint>
#include <cstddef>
#include "Object/Object.h"

class RingBuffer : public Object {
	GENERATED_BODY(RingBuffer, Object)
public:
	RingBuffer(size_t size = 0);
	virtual ~RingBuffer();

	size_t writeAvailable();
	size_t readAvailable();

	size_t read(uint8_t* destination, size_t n);
	size_t write(uint8_t* source, size_t n);

	template<typename T>
	const T* peek(size_t offset = 0){
		if(offset + sizeof(T) > readAvailable()) return nullptr;
		offset = (beginning + offset) % size;
		return (T*) (buffer + offset);
	}

	size_t skip(size_t n);

	void clear();

private:
	size_t size;

	size_t beginning = 0;
	size_t end = 0;

	uint8_t* buffer;
};


#endif //CMF_RINGBUFFER_H
