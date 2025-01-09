#include "ADCUnit.h"

static const char* TAG = "ADC";

ADCUnit* ADCUnit::units[SOC_ADC_PERIPH_NUM] = { nullptr };

ADCUnit::ADCUnit(adc_unit_t unit) : unit(unit){
	const adc_oneshot_unit_init_cfg_t config = {
			.unit_id = unit,
			.clk_src = ADC_RTC_CLK_SRC_DEFAULT,
			.ulp_mode = ADC_ULP_MODE_DISABLE
	};
	ESP_ERROR_CHECK(adc_oneshot_new_unit(&config, &hndl));
}

ADCUnit::~ADCUnit(){
	ESP_ERROR_CHECK(adc_oneshot_del_unit(hndl));
}

void ADCUnit::config(adc_channel_t chan, const adc_oneshot_chan_cfg_t& cfg){
	ESP_ERROR_CHECK(adc_oneshot_config_channel(hndl, chan, &cfg));
}

adc_unit_t ADCUnit::getUnitID() const{
	return unit;
}

esp_err_t ADCUnit::read(adc_channel_t chan, int& valueOut, const adc_cali_handle_t cali) const{
	if(cali != nullptr){
		return adc_oneshot_get_calibrated_result(hndl, cali, chan, &valueOut);
	}else{
		return adc_oneshot_read(hndl, chan, &valueOut);
	}
}

ADCUnit& ADCUnit::getADCUnit(adc_unit_t unit){
	if(!units[unit]) units[unit] = new ADCUnit(unit);

	return *units[unit];
}
