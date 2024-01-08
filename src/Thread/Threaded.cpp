#include "Threaded.h"
#include "Util/stdafx.h"

Threaded::Threaded(const std::string& threadName, TickType_t interval/* = 0*/, size_t threadStackSize/* = 12 * 1024*/, uint8_t threadPriority/* = 5*/, int8_t cpuCore/* = -1*/) noexcept :
		name(threadName), loopInterval(interval), stackSize(threadStackSize), priority(threadPriority), core(cpuCore) {
	stopSemaphore = xSemaphoreCreateBinary();
	stopMutex = xSemaphoreCreateMutex();
	pauseSemaphore = xSemaphoreCreateBinary();
}

Threaded::Threaded(const std::function<void(void)>& fn, const std::string& threadName, TickType_t interval/* = 0*/, size_t threadStackSize/* = 12 * 1024*/, uint8_t threadPriority/* = 5*/, int8_t cpuCore/* = -1*/) noexcept :
		name(threadName), loopInterval(interval), stackSize(threadStackSize), priority(threadPriority), core(cpuCore), lambdaLoop(fn) {
	stopSemaphore = xSemaphoreCreateBinary();
	stopMutex = xSemaphoreCreateMutex();
	pauseSemaphore = xSemaphoreCreateBinary();
}

Threaded::~Threaded() noexcept{
	if(state != State::Stopped){
		// TODO: log error
		abort();
	}

	vSemaphoreDelete(stopSemaphore);
	vSemaphoreDelete(stopMutex);
	vSemaphoreDelete(pauseSemaphore);
}

void Threaded::start() noexcept{
	if(state != State::Stopped){
		return;
	}

	if(!onStart()){
		return;
	}

	state = State::Running;

	// TODO: error code checking and logging, error fallback if possible
	// TODO: check if arg is a non null object pointer that is Threaded before calling the function (after memory functionality is added)
	if(core == -1){
		xTaskCreate([](void* arg) -> void { ((Threaded*) arg)->threadFunction(); }, name.c_str(), stackSize, this, priority, &task);
	}else{
		xTaskCreatePinnedToCore([](void* arg) -> void { ((Threaded*) arg)->threadFunction(); }, name.c_str(), stackSize, this, priority, &task, core);
	}
}

void Threaded::stop(TickType_t wait/* = portMAX_DELAY*/) noexcept{
	if(xSemaphoreTake(stopMutex, wait) == pdFALSE){
		return;
	}

	if(state != State::Running){
		xSemaphoreGive(stopMutex);
		return;
	}

	state = State::Stopping;

	xSemaphoreTake(stopSemaphore, wait);
	xSemaphoreGive(stopMutex);
}

void Threaded::pause() noexcept{
	if(paused) {
		return;
	}

	xSemaphoreGive(pauseSemaphore);

	while(!paused){
		vTaskDelay(1);
	}
}

void Threaded::resume() noexcept{
	paused = false;
	start();
}

bool Threaded::running() const noexcept{
	return state != State::Stopped;
}

bool Threaded::onStart() noexcept{
	return true;
}

bool Threaded::onStop() noexcept{
	return true;
}

void Threaded::loop() noexcept{
	if(!lambdaLoop){
		return;
	}

	lambdaLoop();
}

void Threaded::threadFunction() noexcept{
	while(state == State::Running){
		if(millis() - lastLoop < loopInterval){
			if(xSemaphoreTake(pauseSemaphore, millis() - lastLoop + 1) == pdTRUE){
				stop(0);
				paused = true;
				return;
			}

			return;
		}

		lastLoop = millis();

		loop();
	}

	onStop();

	state = State::Stopped;
	xSemaphoreGive(stopSemaphore);

	vTaskDelete(nullptr);
}