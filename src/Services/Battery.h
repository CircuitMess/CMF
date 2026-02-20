#ifndef CMF_BATTERY_H
#define CMF_BATTERY_H

#include <hal/gpio_types.h>
#include <atomic>
#include <Entity/AsyncEntity.h>
#include <Services/ADCReader.h>
#include <Drivers/Interface/OutputDriver.h>
#include <mutex>
#include <esp_efuse.h>
#include <memory>
#include <utility>
#include <Util/Hysteresis.h>
#include <Event/EventBroadcaster.h>


/**
 * Configuration struct of Battery service
 */
struct BatteryServiceConfig {
	/**
	 * Interval between ADC measurements
	 */
	uint32_t measureInterval;

	/**
	 * Expected voltage minimum in millivolts.
	 * Values lower than this will be considered in "shutdown" range.
	 */
	float voltEmpty;

	/**
	 * Expected voltage maximum in millivolts.
	 * Values above this will be considered in "full charge" range.
	 */
	float voltFull;

	/**
	 * Battery reading pins are usually connected to the power source via a voltage divider.
	 * This factor will be applied to readings for calculating the actual battery voltage.
	 */
	float dividerFactor;

	/**
	 * Sometimes there is a predictable and stable voltage drop between the battery and ADC reading pin (excluding the voltage divider).
	 * The specified offset will be added on-top of the read battery voltage, after the divider factor was applied.
	 */
	float voltOffset;

	/**
	 * ADC readings will inherently contain some noise. To address this, you can set the exponential moving average factor (EMA).
	 * https://en.wikipedia.org/wiki/Exponential_smoothing
	 * Formula used is output = factor * current + (1 - factor) * last_output
	 * Factor value should be in range [0, 1.0]. Usually, a value of 0.2 to 0.05 is adequate.
	 * Value of 1.0 switches off filtering.
	 */
	float EMA_factor;
};

class Battery : public Object {
	GENERATED_BODY(Battery, Object, CONSTRUCTOR_PACK(gpio_num_t, uint8_t, OutputPin))
public:

	/**
	 *
	 * @param config Battery service config
	 * @param battReadPin GPIO pin used for reading, must be input and ADC capable
	 * @param levels Hysteresis for
	 * @param refSwitch
	 */
	Battery(BatteryServiceConfig config, gpio_num_t battReadPin, std::vector<uint8_t> levels, OutputPin refSwitch);

	void begin();

	DECLARE_EVENT(BatteryEvent, Battery, uint8_t);
	BatteryEvent OnLevelChanged{ this };

	bool isShutdown() const;

	float getPerc() const;
	uint8_t getLevel() const;

private:
	const BatteryServiceConfig config;

	static constexpr int CalReads = 10;

	static constexpr float CalExpected = 2500;
	StrongObjectPtr<Threaded> batThread;

	OutputPin refSwitch;

	Hysteresis hysteresis;
	StrongObjectPtr<FactorOffset_ADCFilter> readerBattoffsetFilter;

	StrongObjectPtr<EMA_ADCFilter> readerBattEMAFilter;
	StrongObjectPtr<ADCReader> readerBatt;
	StrongObjectPtr<ADCReader> readerRef;

	void calibrate();

	void sample(bool fresh = false);
	bool shutdown = false;

	void tick() noexcept;
};

#endif //CMF_BATTERY_H
