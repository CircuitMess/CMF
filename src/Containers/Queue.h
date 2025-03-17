#ifndef CMF_QUEUE_H
#define CMF_QUEUE_H

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <cstddef>
#include <cstring>
#include <memory>
#include <mutex>

/**
 * @brief A variable size queue which is thread-safe, template,
 * and supports a custom allocator for all memory allocations.
 * @tparam T The type of data being held in the queue.
 * @tparam Allocator The type of allocator used for memory operations within the queue.
 */
template<typename T, typename Allocator = std::allocator<T>>
class Queue {
public:
	/**
	 * @brief The constructor with starting size option,
	 * @param size The size the queue starts its life with.
	 */
	inline explicit Queue(size_t size = DefaultSize) noexcept : bufferSize(size), waitSemaphore(xSemaphoreCreateBinary()) {
		buffer = allocator.allocate(bufferSize);
	}

	/**
	 * @brief The copy constructor from another queue of same data type.
	 * @param other The queue being copied.
	 */
	inline Queue(const Queue& other) noexcept : bufferSize(other.bufferSize), begin(other.begin), end(other.end), waitSemaphore(xSemaphoreCreateBinary()) {
		buffer = allocator.allocate(bufferSize);

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			buffer[i] = T(std::move_if_noexcept(other.buffer[i]));
		}
	}

	/**
	 * @brief The move constructor from another queue with the same data type.
	 * @param other The queue being moved. The queue is empty after the constructor finishes execution.
	 */
	inline Queue(Queue&& other) noexcept : buffer(other.buffer), bufferSize(other.bufferSize), begin(other.begin), end(other.end), accessMutex(std::move(other.accessMutex)) {
		other.buffer = 0;
		other.bufferSize = 0;
		other.begin = other.end = 0;
		other.accessMutex = nullptr;
		waitSemaphore = other.waitSemaphore;
		other.waitSemaphore = nullptr;
		accessMutex.unlock();
	}

	/**
	 * @brief Unblocks the thread-safe synchronization and frees memory, destroys the queue.
	 */
	inline ~Queue() noexcept {
		setKillPill();

		std::lock_guard guard(accessMutex);

		allocator.deallocate(buffer, bufferSize);
		buffer = nullptr;
		begin = end = 0;
		bufferSize = 0;
	}

	/**
	 * @return The size of the queue.
	 */
	inline size_t size() const noexcept {
		if(begin > end){
			return bufferSize - begin + end + 1;
		}

		return end - begin;
	}

	/**
	 * @return The capacity of the queue (maximum size without resizing).
	 */
	inline size_t capacity() const noexcept {
		return bufferSize;
	}

	/**
	 * @brief Checker for an empty queue.
	 * @return True if size is 0. False otherwise.
	 */
	inline bool empty() const noexcept {
		return begin == end;
	}

	/**
	 * @brief Checker for a full queue.
	 * @return True if size equals capacity. False otherwise.
	 */
	inline bool full() const noexcept {
		return size() >= bufferSize;
	}

	/**
	 * @brief Reserves a capacity of the queue to the given size.
	 * @param newSize The new capacity of the queue.
	 * @return True if successful, false otherwise.
	 */
	inline bool reserve(size_t newSize) noexcept {
		std::lock_guard guard(accessMutex);
		return reserveInternal(newSize);
	}

	/**
	 * @param value The variable that is set to the front value.
	 * @param wait The maximum time to halt thread execution and wait to retrieve a value.
	 * @return True if successful, false otherwise.
	 */
	inline bool front(T& value, TickType_t wait = portMAX_DELAY) noexcept {
		if(xSemaphoreTake(waitSemaphore, wait) != pdTRUE){
			return false;
		}

		std::lock_guard guard(accessMutex);

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

	/**
	 * @brief Pop function which retrieves a value from the internal buffer before removing it internally.
	 * @param value The value variable set to the value at the top of the stack before removing it.
	 * @param wait The maximum wait time for a successful value pop before abort.
	 * @return True if successful, false otherwise.
	 */
	inline bool pop(T& value, TickType_t wait = portMAX_DELAY) noexcept {
		if(xSemaphoreTake(waitSemaphore, wait) != pdTRUE){
			return false;
		}

		std::lock_guard guard(accessMutex);

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

	/**
	 * @brief Push function which adds a value at the end of the queue.
	 * @param value The value being added to the queue.
	 * @return True if successful, false otherwise.
	 */
	inline bool push(const T& value) noexcept {
		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		std::lock_guard guard(accessMutex);

		buffer[end] = T(value);

		end = (end + 1) % bufferSize;

		xSemaphoreGive(waitSemaphore);

		return true;
	}

	/**
	 * @brief A move push function. The give value is moved to the queue and emptied / invalidated.
	 * @param value The value being added to the queue.
	 * @return True if successful, false otherwise.
	 */
	inline bool push(T&& value) noexcept {
		if(full() && !reserve(bufferSize * 2)){
			return false;
		}

		std::lock_guard guard(accessMutex);

		buffer[end] = T(std::move(value));

		end = (end + 1) % bufferSize;

		xSemaphoreGive(waitSemaphore);

		return true;
	}

	/**
	 * @brief Checks if a value is present in the queue.
	 * @param value The value being checked.
	 * @return True if queue contains the given value.
	 */
	inline bool check(const T& value) const noexcept {
		std::lock_guard guard(accessMutex);

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			if(buffer[i] == value) {
				return true;
			}
		}

		return false;
	}

	/**
	 * @brief Removes the given value from the queue no matter the location.
	 * @param value The value being removed.
	 */
	inline void remove(const T& value) noexcept {
		std::lock_guard guard(accessMutex);

		bool found = true;
		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			if(buffer[i] == value){
				found = false;
				buffer[i].~T();
				continue;
			}

			if(!found){
				continue;
			}

			buffer[i-1] = buffer[i];
		}

		if(found){
			--end;
		}
	}

	/**
	 * @brief Removes all values from the queue, leaving it empty.
	 */
	inline void clear() noexcept {
		std::lock_guard guard(accessMutex);

		for(size_t i = begin; i <= end; i = (i + 1) % bufferSize){
			buffer[i].~T();
		}

		end = begin = 0;

		xSemaphoreTake(waitSemaphore, 0);
	}

	/**
	 * @brief Unblocks all thread-safe functionality with a kill pill, meaning all data retrieval attempts will fail, but will unblock threads waiting on it.
	 * @param value The value being set to the kill pill.
	 */
	inline void setKillPill(bool value = true) noexcept {
		std::lock_guard guard(accessMutex);
		kill = value;

		if(kill){
			xSemaphoreGive(waitSemaphore);
		}
	}

private:
	static constexpr size_t DefaultSize = 16;
	T* buffer;
	Allocator allocator = Allocator();
	size_t bufferSize;
	size_t begin = 0;
	size_t end = 0;
	bool kill = false;
	std::mutex accessMutex;
	SemaphoreHandle_t waitSemaphore;

private:
	/**
	 * @brief An internal function used to reserve memory needed for resizing of the queue.
	 * Before resizing, all values that will be lost when downsizing are destroyed first. The rest are moved or copied to the new buffer.
	 * @param newSize The new size of the queue./
	 * @return True if successful, false otherwise.
	 */
	inline bool reserveInternal(size_t newSize) noexcept {
		T* newBuffer = allocator.allocate(newSize);
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

		allocator.deallocate(buffer, bufferSize);
		buffer = newBuffer;
		bufferSize = newSize;

		return true;
	}
};

#endif //CMF_QUEUE_H
