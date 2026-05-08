#ifndef CMF_TIMER_H
#define CMF_TIMER_H

#include "Object/Object.h"
#include "Object/Class.h"
#include <esp_timer.h>

/**
 * Abstraction for ESP's hardware timers
 *
 * Note:
 * ESP Timer HAL takes care of HW allocation when Timers are created. (No need to specify timerID and timerGroup)
 * For more control, use GPTimer instead.
 *
 * Callbacks are dispatched from the high-resolution timer task (ESP_TIMER_TASK), not from ISR
 * context, so they may safely call code that is not in IRAM. Storing a std::function and invoking
 * it from a true ISR is unsafe — both std::function dispatch and captured lambda bodies live in
 * flash and would trigger a cache-disabled crash whenever the timer fires while flash is busy.
 */


class Timer : public Object {
	GENERATED_BODY(Timer, Object, CONSTRUCTOR_PACK(uint32_t, std::function<void()>, const char*))

public:
	/**
	 * @param period Time between periodic callback invocations [ms]
	 * @param ISR Callback to be invoked when the period elapses. Runs in the esp_timer task; should be non-blocking.
	 * @param name Name for internal ESP timer HAL (optional, for debugging purposes)
	 */

	Timer(uint32_t period, std::function<void()> ISR, const char* name = "");
	virtual ~Timer();

	void start();
	void stop();
	void reset();


	/**
	 * Creates a one-time timer, instead of a periodically called one
	 * @param delay Delay before the callback is invoked [ms]
	 * @param ISR Callback to be invoked when the delay elapses. Runs in the esp_timer task; should be non-blocking.
	 */

	static void single(uint32_t delay, std::function<void()> ISR);

	void setPeriod(uint32_t period);

private:
	static void interrupt(void* arg);
	esp_timer_handle_t timer;

	uint64_t period;
	const std::function<void()> ISR = nullptr;

};


#endif //CMF_TIMER_H
