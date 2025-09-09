#include "ShutdownService.h"
#include "Services/Audio/Audio.h"
#include "Enums.h"
#include "Pins.hpp"
#include "Drivers/Output/OutputGPIO.h"
#include <Services/Motors/Motors.h>
#include <Services/LED/LED.h>

void ShutdownService::shutdown(ShutdownReason reason){
	auto app = getApp();

	auto audio = app->getService<Audio>();

	switch(reason){
		case ShutdownReason::Inactivity:
			audio->play("/spiffs/Gasenje.aac");
			break;
		case ShutdownReason::Battery:
			audio->play("/spiffs/GasenjeBatt.aac");
			break;
		default:
			break;
	}

	auto motors = app->getService<Motors<int>>();
	motors->set(0, 0);

	auto leds = app->getService<LED<LEDs, RGB_LEDs>>();


	for(int i = 0; i < (uint8_t) LEDs::COUNT; i++){
		leds->off((LEDs) i);
	}

	for(int i = 0; i < (uint8_t) RGB_LEDs::COUNT; i++){
		leds->off((RGB_LEDs) i);
	}

	delayMillis(1000);

	app->getDriver<OutputGPIO>()->write(SPKR_EN, false);


	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_RC_FAST, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_CPU, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_AUTO));
	ESP_ERROR_CHECK(esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL));
	esp_deep_sleep_start();
}
