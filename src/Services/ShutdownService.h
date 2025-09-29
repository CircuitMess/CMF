#ifndef CMF_SHUTDOWN_SERVICE_H
#define CMF_SHUTDOWN_SERVICE_H

#include <esp_sleep.h>
#include "Event/EventBroadcaster.h"
#include "Entity/AsyncEntity.h"
#include "Services/Battery/Battery.h"

enum class ShutdownReason : uint8_t {
	Inactivity, Battery
};

class ShutdownService : public AsyncEntity {
	GENERATED_BODY(ShutdownService, AsyncEntity)

public:
	DECLARE_EVENT(OnShutdownEvent, ShutdownService, ShutdownReason)
	OnShutdownEvent OnShutdown {this};

public:
	static SemaphoreHandle_t ShutdownSemaphore;

public:
	ShutdownService();

	static constexpr uint32_t InactivityTimeout = CONFIG_CMF_INACTIVITY_TIMEOUT >= 0 ? CONFIG_CMF_INACTIVITY_TIMEOUT : portMAX_DELAY; //[ms]

	void shutdown(ShutdownReason reason);

	template<typename ...Args>
	void activityCallback(Args... args) {
#if CONFIG_CMF_INACTIVITY_TIMEOUT >= 0
		xSemaphoreGive(inactivitySem);
#endif
	}

private:
	SemaphoreHandle_t inactivitySem;

private:
	void tick(float deltaTime) noexcept override;

	void onBatteryLevelChange(Battery::Level level);
};


#endif //CMF_SHUTDOWN_SERVICE_H
