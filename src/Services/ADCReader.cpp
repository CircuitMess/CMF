#include "ADCReader.h"
#include <algorithm>
#include "Log/Log.h"

DEFINE_LOG(ADCReader)

ADCReader::ADCReader(gpio_num_t gpio, adc_oneshot_chan_cfg_t config, ADCFilter* filter, adc_cali_handle_t cali) : filter(filter), cali(cali){
	adc_unit_t unit;
	adc_channel_t chan;
	auto err = adc_oneshot_io_to_channel(gpio, &unit, &chan);
	if(err == ESP_ERR_NOT_FOUND){
		CMF_LOG(ADCReader, Error, "GPIO %d is not a valid ADC pin", gpio);
	}else if(err != ESP_OK){
		CMF_LOG(ADCReader, Error, "ADCReader error");
	}

	this->chan = chan;
	adc = &ADCUnit::getADCUnit(unit);
	adc->config(chan, config);
}

float ADCReader::sample(){
	int raw = 0;
	if(adc->read(chan, raw, cali) != ESP_OK){
		return getValue();
	}

	value = raw;

	return getValue();
}

float ADCReader::getValue() const{
	if(filter){
		return filter->apply(value);
	}

	return value;
}
