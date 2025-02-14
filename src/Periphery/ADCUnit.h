#ifndef CODEE_FIRMWARE_ADC_H
#define CODEE_FIRMWARE_ADC_H

#include "Object/Object.h"
#include <esp_adc/adc_oneshot.h>

class ADCUnit : public Object {
	GENERATED_BODY(ADCUnit, Object)

public:
	static ADCUnit* getADCUnit(adc_unit_t unit);

	explicit ADCUnit(adc_unit_t unit = ADC_UNIT_1);

	virtual ~ADCUnit() override;

	adc_unit_t getUnitID() const;

	void config(adc_channel_t chan, const adc_oneshot_chan_cfg_t& cfg);

	esp_err_t read(adc_channel_t chan, int& valueOut, adc_cali_handle_t cali = nullptr) const;

private:
	adc_oneshot_unit_handle_t hndl;
	const adc_unit_t unit;

	static StrongObjectPtr<ADCUnit> units[SOC_ADC_PERIPH_NUM];
};

#endif //CODEE_FIRMWARE_ADC_H
