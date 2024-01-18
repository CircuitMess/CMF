#ifndef CMF_QUEUE_H
#define CMF_QUEUE_H

#include <cstddef>
#include <cstring>
#include <memory>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// TODO think about support for custom allocators with Queue
template<typename T>
class Queue {
public:
	inline explicit Queue(size_t size = DefaultSize) noexcept : bufferSize(size), accessMutex(xSemaphoreCreateMutex()) {
		buffer = new T[bufferSize];
	}

	inline Queue(const Queue& other) noexcept : bufferSize(other.bufferSize), begin(other.begin), end(other.end), accessMutex(xSemaphoreCreateMutex()) {
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
		if(accessMutex != nullptr){
			vSemaphoreDelete(accessMutex);
			accessMutex = nullptr;
		}

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
			return end + bufferSize - begin;
		}

		return end - begin;
	}

	inline bool reserve(size_t newSize, TickType_t wait = portMAX_DELAY) noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		T* newBuffer = new T[newSize];
		if(buffer == nullptr){
			return false;
		}

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			newBuffer[newSize - bufferSize + i] =  T(std::move_if_noexcept(buffer[i]));
		}

		if(begin > end){
			begin = newSize - bufferSize + begin;
		}

		delete[] buffer;
		buffer = newBuffer;
		bufferSize = newSize;

		xSemaphoreGive(accessMutex);

		return false;
	}

	inline bool front(T& value, TickType_t wait = portMAX_DELAY) const noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		if(empty()){
			return false;
		}

		value = buffer[begin];

		xSemaphoreGive(accessMutex);

		return true;
	}

	inline bool pop(T& value, TickType_t wait = portMAX_DELAY) noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		if(empty()){
			return false;
		}

		value = buffer[begin];
		buffer[begin].~T();

		begin = (begin + 1) % bufferSize;

		xSemaphoreGive(accessMutex);

		return true;
	}

	inline bool push(const T& value, TickType_t wait = portMAX_DELAY) noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		end = (end + 1) % bufferSize;

		buffer[end] = T(value);

		xSemaphoreGive(accessMutex);

		return true;
	}

	inline bool push(T&& value, TickType_t wait = portMAX_DELAY) noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		end = (end + 1) % bufferSize;

		buffer[end] = T(value);

		xSemaphoreGive(accessMutex);

		return true;
	}

	inline bool clear(TickType_t wait = portMAX_DELAY) noexcept {
		if(accessMutex == nullptr){
			return false;
		}

		if(xSemaphoreTake(accessMutex, wait) != pdTRUE){
			return false;
		}

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			buffer[i].~T();
		}

		end = begin = 0;

		xSemaphoreGive(accessMutex);
	}

private:
	static constexpr size_t DefaultSize = 32;
	T* buffer;
	size_t bufferSize;
	size_t begin = 0;
	size_t end = 0;
	SemaphoreHandle_t accessMutex;
};

#endif //CMF_QUEUE_H
