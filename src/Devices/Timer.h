#ifndef CMF_TIMER_H
#define CMF_TIMER_H

#include "Object/Object.h"
#include "Object/Class.h"
#include <esp_timer.h>

/**
 * Abstraction for ESP's hardware timers (esp_timer with ISR dispatch).
 *
 * Note:
 * ESP Timer HAL takes care of HW allocation when Timers are created. (No need to specify timerID and timerGroup).
 * For more control, use GPTimer instead.
 *
 * Dispatch path: the underlying esp_timer fires the trampoline in ISR context (ESP_TIMER_ISR),
 * preserving the low alarm-to-wake-up latency. The trampoline lives in IRAM and only signals a
 * shared high-priority worker task via an ISR-safe queue; the user's std::function callback runs
 * on that task. This keeps every flash-resident dispatch step (std::function invoker, captured
 * lambda body) outside the cache-disabled ISR window, so a timer firing during a flash write no
 * longer triggers a cache-disabled crash.
 *
 * Implication: callbacks may run shortly after Timer::stop() / ~Timer() returns if an item was
 * already queued from the ISR. Callers must not rely on synchronous shutdown.
 */


class Timer : public Object {
	GENERATED_BODY(Timer, Object, CONSTRUCTOR_PACK(uint32_t, std::function<void()>, const char*))

public:
	/**
	 * @param period Time between periodic callback invocations [ms]
	 * @param ISR Callback to be invoked when the period elapses. Runs in the Timer worker task; should be non-blocking.
	 * @param name Name for internal ESP timer HAL (optional, for debugging purposes)
	 */

	Timer(uint32_t period, std::function<void()> ISR, const char* name = "");
	virtual ~Timer();

	void start();
	void stop();
	void reset();


	/**
	 * Creates a one-time timer, instead of a periodically called one.
	 * @param delay Delay before the callback is invoked [ms]
	 * @param ISR Callback to be invoked when the delay elapses. Runs in the Timer worker task; should be non-blocking.
	 */

	static void single(uint32_t delay, std::function<void()> ISR);

	void setPeriod(uint32_t period);

private:
	static void IRAM_ATTR interrupt(void* arg);
	esp_timer_handle_t timer;

	uint64_t period;
	std::function<void()> ISR = nullptr;

};


#endif //CMF_TIMER_H
