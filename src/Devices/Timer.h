#ifndef CMF_TIMER_H
#define CMF_TIMER_H

#include "Object/Object.h"
#include "Object/Class.h"
#include <esp_timer.h>

/**
 * Abstraction for ESP's hardware timers (esp_timer with ISR dispatch).
 *
 * The underlying esp_timer is created with dispatch_method = ESP_TIMER_ISR, so the user
 * callback runs directly in ISR context. For this to work the application must enable
 * CONFIG_ESP_TIMER_SUPPORTS_ISR_DISPATCH_METHOD in its sdkconfig.
 *
 * Because the callback runs in ISR context with the SPI flash cache potentially disabled:
 *   - The callback function must be marked IRAM_ATTR.
 *   - Any constants the callback reads must live in DRAM (DRAM_ATTR / static const).
 *   - The callback must not call into flash-resident code or perform blocking work.
 *   - Only ISR-safe FreeRTOS primitives (the *FromISR variants) may be used.
 *
 * ESP Timer HAL takes care of HW allocation when Timers are created (no need to specify
 * timerID and timerGroup). For more control, use GPTimer instead.
 */


class Timer : public Object {
	GENERATED_BODY(Timer, Object, CONSTRUCTOR_PACK(uint32_t, void(*)(void*), void*, const char*))

public:
	using Callback = void (*)(void* arg);

	/**
	 * @param period Time between periodic callback invocations [ms]
	 * @param callback Callback to be invoked when the period elapses. Runs in ISR context — must be IRAM_ATTR.
	 * @param arg Opaque pointer forwarded to callback on every invocation.
	 * @param name Name for internal ESP timer HAL (optional, for debugging purposes)
	 */
	Timer(uint32_t period, Callback callback, void* arg, const char* name = "");
	virtual ~Timer();

	void start();
	void stop();
	void reset();

	/**
	 * Creates a one-time timer, instead of a periodically called one.
	 * @param delay Delay before the callback is invoked [ms]
	 * @param callback Callback to be invoked when the delay elapses. Runs in ISR context — must be IRAM_ATTR.
	 * @param arg Opaque pointer forwarded to callback when it fires.
	 */
	static void single(uint32_t delay, Callback callback, void* arg);

	void setPeriod(uint32_t period);

private:
	esp_timer_handle_t timer;
	uint64_t period;
};


#endif //CMF_TIMER_H
