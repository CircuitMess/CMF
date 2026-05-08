#include "Timer.h"
#include "Log/Log.h"

#include <utility>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

DEFINE_LOG(Timer)

namespace {

struct TimerWork {
	const std::function<void()>* callback;
	bool deleteAfter;
};

QueueHandle_t g_workQueue = nullptr;
TaskHandle_t g_workerTask = nullptr;

void timerWorkerTask(void*){
	TimerWork work{};
	for(;;){
		if(xQueueReceive(g_workQueue, &work, portMAX_DELAY) != pdTRUE){
			continue;
		}
		if(work.callback != nullptr && *work.callback){
			(*work.callback)();
		}
		if(work.deleteAfter){
			delete work.callback;
		}
	}
}

void ensureWorkerStarted(){
	static const bool started = []() {
		g_workQueue = xQueueCreate(CONFIG_CMF_TIMER_QUEUE_LENGTH, sizeof(TimerWork));
		if(g_workQueue == nullptr){
			CMF_LOG(Timer, LogLevel::Error, "Failed to create Timer worker queue");
			abort();
		}
		const auto ret = xTaskCreate(
			timerWorkerTask, "CMF-Timer", CONFIG_CMF_TIMER_STACK_SIZE,
			nullptr, CONFIG_CMF_TIMER_THREAD_PRIORITY, &g_workerTask);
		if(ret != pdPASS){
			CMF_LOG(Timer, LogLevel::Error, "Failed to create Timer worker task (ret=%d)", (int) ret);
			abort();
		}
		return true;
	}();
	(void) started;
}

void IRAM_ATTR sendWorkFromISR(const std::function<void()>* cb, bool deleteAfter){
	TimerWork work;
	work.callback = cb;
	work.deleteAfter = deleteAfter;

	BaseType_t higherPriorityTaskWoken = pdFALSE;
	xQueueSendFromISR(g_workQueue, &work, &higherPriorityTaskWoken);
	if(higherPriorityTaskWoken == pdTRUE){
		portYIELD_FROM_ISR();
	}
}

void IRAM_ATTR singleTrampoline(void* arg){
	auto* cb = static_cast<std::function<void()>*>(arg);
	sendWorkFromISR(cb, true);
}

} // namespace

Timer::Timer(uint32_t period, std::function<void()> ISR, const char* name) : period(period * 1000), ISR(std::move(ISR)){
	ensureWorkerStarted();

	char timerName[32];
	sprintf(timerName, "Tmr-%s", name);

	esp_timer_create_args_t args = {
			.callback = interrupt,
			.arg = this,
			.dispatch_method = ESP_TIMER_ISR,
			.name = timerName,
			.skip_unhandled_events = true
	};
	ESP_ERROR_CHECK(esp_timer_create(&args, &timer));
}

Timer::~Timer(){
	stop();
	esp_timer_delete(timer);
}

void Timer::start(){
	esp_timer_start_once(timer, period);
}

void Timer::stop(){
	esp_timer_stop(timer);
}

void Timer::reset(){
	esp_timer_stop(timer);
	esp_timer_start_once(timer, period);

}

void IRAM_ATTR Timer::interrupt(void* arg){
	auto* self = static_cast<Timer*>(arg);
	sendWorkFromISR(&self->ISR, false);
}

void Timer::single(uint32_t delay, std::function<void()> ISR){
	ensureWorkerStarted();

	char name[32];
	sprintf(name, "Single-%d", rand() % 64);

	auto* func = new std::function<void()>(std::move(ISR));

	esp_timer_create_args_t args = {
			.callback = singleTrampoline,
			.arg = func,
			.dispatch_method = ESP_TIMER_ISR,
			.name = name,
			.skip_unhandled_events = true
	};

	esp_timer_handle_t timer;
	ESP_ERROR_CHECK(esp_timer_create(&args, &timer));

	esp_timer_start_once(timer, delay * 1000);
}

void Timer::setPeriod(uint32_t period){
	if(esp_timer_is_active(timer)){
		CMF_LOG(Timer, LogLevel::Error, "setPeriod called while timer is running");
		return;
	}

	this->period = period*1000;
}
