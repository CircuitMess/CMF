#ifndef CMF_QUEUE_H
#define CMF_QUEUE_H

#include <cstddef>
#include <cstring>
#include <memory>
#include <mutex>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// TODO think about support for custom allocators with Queue
template<typename T>
class Queue {
public:
	inline explicit Queue(size_t size = DefaultSize) noexcept : bufferSize(size), waitSemaphore(xSemaphoreCreateBinary()) {
		buffer = new T[bufferSize];
	}

	inline Queue(const Queue& other) noexcept : bufferSize(other.bufferSize), begin(other.begin), end(other.end), waitSemaphore(xSemaphoreCreateBinary()) {
		buffer = new T[bufferSize];

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			buffer[i] =  T(std::move_if_noexcept(other.buffer[i]));
		}
	}

	inline Queue(Queue&& other) noexcept : buffer(other.buffer), bufferSize(other.bufferSize), begin(other.begin), end(other.end), accessMutex(other.accessMutex) {
		other.buffer = 0;
		other.bufferSize = 0;
		other.begin = other.end = 0;
		other.accessMutex = nullptr;
	}

	inline ~Queue() noexcept {
		setKillPill();

		std::lock_guard guard(accessMutex);

		delete[] buffer;
		buffer = nullptr;
		begin = end = 0;
	}

	inline size_t size() const noexcept {
		return end - begin + 1;
	}

	inline size_t capacity() const noexcept {
		return bufferSize;
	}

	inline bool empty() const noexcept {
		return begin == end;
	}

	inline bool full() const noexcept {
		if(begin > end){
			return end + bufferSize - begin >= bufferSize - 1;
		}

		return end - begin >= bufferSize - 1;
	}

	inline bool reserve(size_t newSize) noexcept {
		std::lock_guard guard(accessMutex);
		return reserveInternal(newSize);
	}

	inline bool front(T& value, TickType_t wait = portMAX_DELAY) const noexcept {
		std::lock_guard guard(accessMutex);

		if(xSemaphoreTake(waitSemaphore, wait) != pdTRUE){
			return false;
		}

		if(kill){
			return false;
		}

		if(empty()){
			return false;
		}

		value = buffer[begin];

		xSemaphoreGive(waitSemaphore);

		return true;
	}

	inline bool pop(T& value, TickType_t wait = portMAX_DELAY) noexcept {
		std::lock_guard guard(accessMutex);

		if(xSemaphoreTake(waitSemaphore, wait) != pdTRUE){
			return false;
		}

		if(kill){
			return false;
		}

		if(empty()){
			return false;
		}

		value = buffer[begin];
		buffer[begin].~T();

		begin = (begin + 1) % bufferSize;

		if(!empty()){
			xSemaphoreGive(waitSemaphore);
		}

		return true;
	}

	inline bool push(const T& value) noexcept {
		std::lock_guard guard(accessMutex);

		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		buffer[end] = T(value);

		end = (end + 1) % bufferSize;

		xSemaphoreGive(waitSemaphore);

		return true;
	}

	inline bool push(T&& value) noexcept {
		std::lock_guard guard(accessMutex);

		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		buffer[end] = T(std::move(value));

		end = (end + 1) % bufferSize;

		xSemaphoreGive(waitSemaphore);

		return true;
	}

	inline void clear() noexcept {
		std::lock_guard guard(accessMutex);

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			buffer[i].~T();
		}

		end = begin = 0;

		xSemaphoreTake(waitSemaphore, 0);
	}

	inline void setKillPill(bool value = true) noexcept {
		std::lock_guard guard(accessMutex);
		kill = value;

		if(kill){
			xSemaphoreGive(waitSemaphore);
		}
	}

private:
	static constexpr size_t DefaultSize = 32;
	T* buffer;
	size_t bufferSize;
	size_t begin = 0;
	size_t end = 0;
	bool kill = false;
	std::mutex accessMutex;
	SemaphoreHandle_t waitSemaphore;

private:
	inline bool reserveInternal(size_t newSize) noexcept {
		T* newBuffer = new T[newSize];
		if(buffer == nullptr){
			return false;
		}

		if(begin > end){
			for(size_t i = begin; i < bufferSize; ++i){
				newBuffer[newSize - bufferSize + i] =  T(std::move_if_noexcept(buffer[i]));
			}

			for(size_t i = 0; i < end; ++i){
				newBuffer[i] =  T(std::move_if_noexcept(buffer[i]));
			}

			begin = newSize - bufferSize + begin;
		}else{
			for(size_t i = begin; i < end; ++i){
				newBuffer[newSize - bufferSize + i] =  T(std::move_if_noexcept(buffer[i]));
			}
		}

		delete[] buffer;
		buffer = newBuffer;
		bufferSize = newSize;

		return true;
	}
};

#endif //CMF_QUEUE_H
