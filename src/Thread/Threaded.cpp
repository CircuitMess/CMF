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

void Threaded::onCreated() noexcept{
	Super::onCreated();

	start();
}


void Threaded::onDestroy() noexcept{
	stop(portMAX_DELAY);

	Super::onDestroy();
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

void Threaded::setInterval(TickType_t value) noexcept{
	loopInterval = value;
}

TickType_t Threaded::getInterval() const noexcept{
	return loopInterval;
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
		TickType_t semaphoreWait = 0;

		if(millis() - lastLoop < loopInterval){
			semaphoreWait = loopInterval - (millis() - lastLoop + 1);
		}

		if(xSemaphoreTake(pauseSemaphore, semaphoreWait) == pdTRUE){
			stop(0);
			paused = true;
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
