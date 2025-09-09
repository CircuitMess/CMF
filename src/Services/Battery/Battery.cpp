#include "Battery.h"
#include "../Pins.hpp"
#include "Memory/ObjectMemory.h"
#include <Util/stdafx.h>
#include <driver/gpio.h>


DEFINE_LOG(Battery)

Battery::Battery(OutputPin refSwitch) : refSwitch(refSwitch), hysteresis({ 0, 4, 15, 30, 50, 70, 90, 100 }, 3){
	adc_oneshot_chan_cfg_t cfg = {
			.atten = ADC_ATTEN_DB_2_5,
			.bitwidth = ADC_BITWIDTH_12
	};

	readerBattoffsetFilter = newObject<FactorOffset_ADCFilter>(this, Factor, Offset);
	readerBattEMAFilter = newObject<EMA_ADCFilter>(this, EmaA);
	std::vector filters = {
			StrongObjectPtr<ADCFilter>{ readerBattEMAFilter },
			StrongObjectPtr<ADCFilter>{ readerBattoffsetFilter },
			StrongObjectPtr<ADCFilter>{ newObject<Remap_ADCFilter>(this, VoltEmpty, VoltFull) }
	};

	readerBatt = newObject<ADCReader>(this, (gpio_num_t) PIN_BATT, cfg, true, newObject<Composite_ADCFilter>(this, filters).get());

	readerRef = newObject<ADCReader>(this, (gpio_num_t) PIN_BATT, cfg, true, newObject<FactorOffset_ADCFilter>(this, Factor, Offset).get());

	calibrate();

	sample(true);

	batThread = newObject<Threaded>(this, [this](){ tick(); }, "Battery", MeasureIntverval, 3 * 1024, 5, 1);
}

float Battery::getPerc() const{
	return readerBatt->getValue();
}

Battery::Level Battery::getLevel() const{
	return static_cast<Level>(hysteresis.get());
}

bool Battery::isShutdown() const{
	return shutdown;
}

void Battery::calibrate() const {
	refSwitch.driver->write(refSwitch.port, true);

	delayMillis(100);
	for(int i = 0; i < CalReads; i++){
		readerRef->sample();
		delayMillis(10);
	}

	float total = 0;
	for(int i = 0; i < CalReads; i++){
		total += readerRef->sample();
		delayMillis(10);
	}

	const float reading = total / (float) CalReads;
	const float offset = CalExpected - reading;
	readerBattoffsetFilter->setOffset(readerBattoffsetFilter->getOffset() + offset);

	refSwitch.driver->write(refSwitch.port, false);

	CMF_LOG(Battery, LogLevel::Info, "Calibration: Read %.02f mV, expected %.02f mV. Applying %.02f mV offset.\n", reading, CalExpected, offset);
}

void Battery::sample(bool fresh){
	if(shutdown) return;

	auto oldLevel = getLevel();

	if(fresh){
		readerBattEMAFilter->reset();
		hysteresis.reset(readerBatt->sample());
	}else{
		hysteresis.update(readerBatt->sample());
	}

	if(oldLevel != getLevel() || fresh){
		OnLevelChanged.broadcast(getLevel());
	}

	if(getLevel() == Level::Critical){
		shutdown = true;
		return;
	}
}

void Battery::tick() noexcept{
	if(shutdown) return;
	sample();
}

void Battery::begin() const {
	batThread->start();
}
