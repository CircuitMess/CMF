#ifndef CODEE_ADCREADER_H
#define CODEE_ADCREADER_H

#include "Periphery/ADCUnit.h"
#include "driver/gpio.h"

class ADCFilter {
public:
	virtual uint16_t apply(uint16_t sample) = 0;
};

class ADCReader : public Object {
	GENERATED_BODY(ADCReader, Object)

public:
	/**
	 * Calibration is first applied (if present), then ADCFilter.
	 * @param gpio GPIO pin
	 * @param filter optional filter applied after reading
	 * @param cali Calibration data
	 */
	ADCReader(gpio_num_t gpio = GPIO_NUM_NC, adc_oneshot_chan_cfg_t config = {}, ADCFilter* filter = nullptr, adc_cali_handle_t cali = nullptr);

	/** Sample and return new value. */
	float sample();

	/** Return current value without sampling. */
	float getValue() const;

private:
	ADCUnit* adc;
	adc_channel_t chan;
	ADCFilter* filter;
	const adc_cali_handle_t cali;

	float value = -1.0f;

};


#endif //CODEE_ADCREADER_H
