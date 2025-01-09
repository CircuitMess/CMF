#ifndef CMF_ADCREADER_H
#define CMF_ADCREADER_H

#include "Periphery/ADCUnit.h"
#include "driver/gpio.h"
#include "Object/Interface.h"
#include "Log/Log.h"

class ADCFilter : public Object {
	GENERATED_BODY(ADCFilter, Object)
public:
	virtual uint16_t apply(uint16_t sample){
		return sample;
	};
};

class EMA_ADCFilter : public ADCFilter {
	GENERATED_BODY(EMA_ADCFilter, ADCFilter)
public:
	EMA_ADCFilter(float factor = 1.0) : factor(factor){};

	uint16_t apply(uint16_t sample) override{
		if(last == UINT16_MAX){
			last = sample;
			return last;
		}

		const auto val = (uint16_t) (factor * sample + (1.0f - factor) * last);
		last = sample;
		return val;
	}

private:
	float factor;
	uint16_t last = UINT16_MAX;
};

class ADCReader : public Object {
	GENERATED_BODY(ADCReader, Object)

public:
	/**
	 * Calibration is first applied (if present), then ADCFilter.
	 * @param gpio GPIO pin
	 * @param cali Turn on/off internal ESP calibration
	 * @param filter optional filter applied after reading, must be an ADCFilter implementation
	 */
	ADCReader(gpio_num_t gpio = GPIO_NUM_NC, adc_oneshot_chan_cfg_t config = {}, bool calibration = false, Object* filter = nullptr);

	/** Sample and return new value. */
	float sample();

	/** Return current value without sampling. */
	float getValue() const;

private:
	ADCUnit* adc;
	adc_channel_t chan;
	StrongObjectPtr<ADCFilter> filter;
	adc_cali_handle_t cali_handle = nullptr;

	float value = -1.0f;

};


#endif //CMF_ADCREADER_H
