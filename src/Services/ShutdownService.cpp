#include "ShutdownService.h"

SemaphoreHandle_t ShutdownService::ShutdownSemaphore = xSemaphoreCreateBinary();

ShutdownService::ShutdownService() : Super(InactivityTimeout, 3 * 1024, 10, -1) {
    inactivitySem = xSemaphoreCreateBinary();
    xSemaphoreGive(inactivitySem);

    xSemaphoreGive(ShutdownSemaphore);

#ifdef CONFIG_CMF_BATTERY_SHUTDOWN
    const Application* app = getApp();
    if(app == nullptr) {
        return;
    }

    Battery* battery = app->getService<Battery>();
    if(!isValid(battery)) {
        return;
    }

    battery->OnLevelChanged.bind(this, &ShutdownService::onBatteryLevelChange);
#endif
}

void ShutdownService::shutdown(ShutdownReason reason) {
    xSemaphoreTake(ShutdownSemaphore, portMAX_DELAY);

    OnShutdown.broadcast(reason);
}

void ShutdownService::tick(float deltaTime) noexcept {
    Super::tick(deltaTime);

#if CONFIG_CMF_INACTIVITY_TIMEOUT >= 0
    if(xSemaphoreTake(inactivitySem, 0) == pdFALSE){
        //Semaphore not given in the last 'Timeout' milliseconds by the inputEvent callback.
        shutdown(ShutdownReason::Inactivity);
    }
#endif
}

void ShutdownService::onBatteryLevelChange(Battery::Level level) {
#ifdef CONFIG_CMF_BATTERY_SHUTDOWN
    if(level == Battery::Level::Critical) {
        shutdown(ShutdownReason::Battery);
    }
#endif
}
