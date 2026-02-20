#ifndef CMF_ADCREADER_H
#define CMF_ADCREADER_H

#include "Object/Class.h"
#include "Periphery/ADCUnit.h"
#include "driver/gpio.h"
#include "Object/Interface.h"
#include "Log/Log.h"

class ADCFilter : public Object {
	GENERATED_BODY(ADCFilter, Object, void)
public:
	virtual float apply(float sample){
		return sample;
	};
};

class EMA_ADCFilter : public ADCFilter {
	GENERATED_BODY(EMA_ADCFilter, ADCFilter, CONSTRUCTOR_PACK(float))
public:
	EMA_ADCFilter(float factor = 1.0) : factor(factor){};

	float apply(float sample) override{
		if(last == -1){
			last = sample;
			return last;
		}

		const auto val = factor * sample + (1.0f - factor) * last;
		last = val;
		return val;
	}

	/**
	 * Resets EMA filter and takes a fresh sample on the next call.
	 */
	void reset(){
		last = -1;
	}

private:
	float factor;
	float last = -1;
};

/**
 * First multiplies the result by given factor, then adds the offset.
 * Useful for converting from voltage divider output to actual source voltage.
 */
class FactorOffset_ADCFilter : public ADCFilter {
	GENERATED_BODY(FactorOffset_ADCFilter, ADCFilter, CONSTRUCTOR_PACK(float, float))
public:
	FactorOffset_ADCFilter(float factor, float offset) : factor(factor), offset(offset){

	}

	float apply(float sample) override{
		return sample * factor + offset;
	}

	float getOffset() const{
		return offset;
	}

	void setOffset(float offset){
		FactorOffset_ADCFilter::offset = offset;
	}

	float getFactor() const{
		return factor;
	}

	void setFactor(float factor){
		FactorOffset_ADCFilter::factor = factor;
	}

private:
	float factor = 1.0f;
	float offset = 0.f;
};

/**
 * Converts value from the specified range to [0.0 - 100.0]
 */
class Remap_ADCFilter : public ADCFilter {
	GENERATED_BODY(Remap_ADCFilter, ADCFilter, CONSTRUCTOR_PACK(float, float));
public:
	Remap_ADCFilter(float min, float max) : min(min), max(max){

	}

	float apply(float sample) override{
		if(min == max) return Super::apply(sample);

		sample = std::clamp(sample, min, max);

		return (sample - min) / (max - min) * 100.0f;
	}

private:
	float min = 0, max = 0;
};

class Composite_ADCFilter : public ADCFilter {
	GENERATED_BODY(Composite_ADCFilter, ADCFilter, CONSTRUCTOR_PACK(std::vector<StrongObjectPtr<ADCFilter>>))
public:
	Composite_ADCFilter(std::vector<StrongObjectPtr<ADCFilter>> filters) : filters(std::move(filters)){

	}

	float apply(float sample) override{
		for(auto& filter: filters){
			sample = filter->apply(sample);
		}

		return sample;
	}

private:
	std::vector<StrongObjectPtr<ADCFilter>> filters;
};


class ADCReader : public Object {
	GENERATED_BODY(ADCReader, Object, CONSTRUCTOR_PACK(gpio_num_t, adc_oneshot_chan_cfg_t, bool, StrongObjectPtr<ADCFilter>))

public:
	/**
	 * Calibration is first applied (if present), then ADCFilter.
	 * @param gpio GPIO pin
	 * @param cali Turn on/off internal ESP calibration
	 * @param filter optional filter applied after reading, must be an ADCFilter implementation
	 */
	ADCReader(gpio_num_t gpio = GPIO_NUM_NC, adc_oneshot_chan_cfg_t config = {}, bool calibration = false, StrongObjectPtr<ADCFilter> filter = nullptr);

	/** Sample and return new value. */
	float sample();

	/** Return current value without sampling. */
	float getValue() const;

private:
	StrongObjectPtr<ADCUnit> adc;
	adc_channel_t chan;
	StrongObjectPtr<ADCFilter> filter;
	adc_cali_handle_t cali_handle = nullptr;

	float value = -1.0f;

};


#endif //CMF_ADCREADER_H
