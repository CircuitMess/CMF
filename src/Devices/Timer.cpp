#include "Timer.h"
#include <esp_log.h>

#include <utility>

static const char* TAG = "Timer";

Timer::Timer(uint32_t period, std::function<void()> ISR, const char* name) : period(period * 1000), ISR(std::move(ISR)){
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

void IRAM_ATTR Timer::start(){
	esp_timer_start_once(timer, period);
}

void IRAM_ATTR Timer::stop(){
	esp_timer_stop(timer);
}

void Timer::reset(){
	esp_timer_stop(timer);
	esp_timer_start_once(timer, period);

}

void IRAM_ATTR Timer::interrupt(void* arg){
	auto timer = (Timer*) arg;
	timer->ISR();
}

void Timer::single(uint32_t delay, std::function<void()> ISR){
	char name[32];
	sprintf(name, "Single-%d", rand() % 64);

	auto func = new std::function<void()>();
	*func = std::move(ISR);

	esp_timer_create_args_t args = {
			.callback = [](void* arg){
				auto func = (std::function<void()>*) arg;
				(*func)();
				delete func;
			},
			.arg = func,
			.dispatch_method = ESP_TIMER_ISR,
			.name = name,
			.skip_unhandled_events = true
	};

	esp_timer_handle_t timer;
	ESP_ERROR_CHECK(esp_timer_create(&args, &timer));

	esp_timer_start_once(timer, delay * 1000);
}
