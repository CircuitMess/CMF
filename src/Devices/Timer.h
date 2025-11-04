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
 */


class Timer : public Object {
	GENERATED_BODY(Timer, Object, void)

public:
	Timer() = default;
	/**
	 * @param period Time between periodic ISR calls [ms]
	 * @param ISR ISR routine to be called when period time runs out. Must be non-blocking (e.g. using fromISR functions)
	 * @param name Name for internal ESP timer HAL (optional, for debugging purposes)
	 */

	Timer(uint32_t period, std::function<void()> ISR, const char* name = "");
	virtual ~Timer();

	void start();
	void stop();
	void reset();


	/**
	 * Creates a one-time timer, instead of a periodically called one
	 * @param delay Time between periodic ISR calls [ms]
	 * @param ISR ISR routine to be called when delay time runs out. Must be non-blocking (e.g. using fromISR functions)
	 */

	static void single(uint32_t delay, std::function<void()> ISR);

private:
	static void interrupt(void* arg);
	esp_timer_handle_t timer;

	uint64_t period;
	const std::function<void()> ISR = nullptr;

};


#endif //CMF_TIMER_H
