#ifndef CMF_SHUTDOWN_SERVICE_H
#define CMF_SHUTDOWN_SERVICE_H

#include <esp_sleep.h>
#include "Object/Object.h"

enum class ShutdownReason : uint8_t {
	Inactivity, Battery
};

class ShutdownService : public Object {
	GENERATED_BODY(ShutdownService, Object)
public:
	// TODO figure out how tf to make this work per project with timeout in the shutdown service and not in the project code
	static constexpr uint32_t InactivityTimeout = 2 * 60000; //[ms] = 2 mins // TODO this shit should somehow be defined per project

	//Hardware shutdown, with notification audio beforehand
	static void shutdown(ShutdownReason reason);
};


#endif //CMF_SHUTDOWN_SERVICE_H
