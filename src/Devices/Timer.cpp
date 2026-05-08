#include "Timer.h"
#include "Log/Log.h"

DEFINE_LOG(Timer)

Timer::Timer(uint32_t period, Callback callback, void* arg, const char* name) : period(period * 1000){
	char timerName[32];
	sprintf(timerName, "Tmr-%s", name);

	esp_timer_create_args_t args = {
			.callback = callback,
			.arg = arg,
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

void Timer::single(uint32_t delay, Callback callback, void* arg){
	char name[32];
	sprintf(name, "Single-%d", rand() % 64);

	esp_timer_create_args_t args = {
			.callback = callback,
			.arg = arg,
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
