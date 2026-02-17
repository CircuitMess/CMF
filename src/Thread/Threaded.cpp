#include "Threaded.h"
#include "Util/stdafx.h"
#include "Log/Log.h"
#include "Memory/ObjectMemory.h"

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
	stop(portMAX_DELAY);

	if(state != State::Stopped){
		CMF_LOG(CMF, LogLevel::Error, "Thread was destroyed before being stopped.");
		abort();
	}

	vSemaphoreDelete(stopSemaphore);
	vSemaphoreDelete(stopMutex);
	vSemaphoreDelete(pauseSemaphore);
}

void Threaded::__postInitProperties() noexcept{
	Super::__postInitProperties();

	start();
}

void Threaded::start() noexcept{
	if(state != State::Stopped){
		return;
	}

	if(!onStart()){
		return;
	}

	state = State::Running;

	auto function = [](void* arg) -> void {
		if(arg == nullptr){
			CMF_LOG(CMF, Error, "Thread native callback started with invalid thread pointer.");
			vTaskDelete(nullptr);
			return;
		}

		if(!isValid(static_cast<Threaded *>(arg))){
			CMF_LOG(CMF, Error, "Thread native callback started with invalid thread pointer.");
			vTaskDelete(nullptr);
			return;
		}

		static_cast<Threaded *>(arg)->threadFunction();
	};

	if(core == -1){
		const auto ret = xTaskCreate(function, name.c_str(), stackSize, this, priority, &task);
		assert(ret == pdPASS);
	}else{
		const auto ret = xTaskCreatePinnedToCore(function, name.c_str(), stackSize, this, priority, &task, core);
		assert(ret == pdPASS);
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

		if(xSemaphoreTake(pauseSemaphore, std::max(semaphoreWait, static_cast<TickType_t>(1))) == pdTRUE){
			stop(0);
			paused = true;
			break;
		}

		lastLoop = millis();

		loop();
	}

	onStop();

	state = State::Stopped;
	xSemaphoreGive(stopSemaphore);

	vTaskDelete(nullptr);
}
