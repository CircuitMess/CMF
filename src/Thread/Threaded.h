#ifndef CMF_THREADED_H
#define CMF_THREADED_H

#include "Object/Object.h"
#include <cstddef>
#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <functional>

class Threaded : public Object {
	GENERATED_BODY(Threaded, Object)

public:
	explicit Threaded(const std::string& threadName = "", TickType_t interval = 0, size_t threadStackSize = 12 * 1024, uint8_t threadPriority = 5, int8_t cpuCore = -1) noexcept;
	Threaded(const std::function<void(void)>& fn, const std::string& threadName, TickType_t interval = 0, size_t threadStackSize = 12 * 1024, uint8_t threadPriority = 5, int8_t cpuCore = -1) noexcept;
	virtual ~Threaded() noexcept;

public:
	virtual void onCreated() noexcept override;

	void start() noexcept;
	void stop(TickType_t wait = portMAX_DELAY) noexcept;
	void pause() noexcept;
	void resume() noexcept;

	bool running() const noexcept;

public:
	virtual bool onStart() noexcept;
	virtual bool onStop() noexcept;
	virtual void loop() noexcept;

private:
	enum class State : uint8_t {
		Running,
		Stopping,
		Stopped
	};

private:
	std::string name;
	const TickType_t loopInterval;
	size_t stackSize;
	const uint8_t priority;
	const int8_t core;

	State state = State::Stopped;
	bool paused = false;
	TickType_t lastLoop = 0;
	std::function<void(void)> lambdaLoop = nullptr;

	TaskHandle_t task = nullptr;
	SemaphoreHandle_t stopSemaphore;
	SemaphoreHandle_t stopMutex;
	SemaphoreHandle_t pauseSemaphore;

private:
	void threadFunction() noexcept;
};

#endif //CMF_THREADED_H