#include "Application.h"

Application::Application(TickType_t interval /*= 0*/, size_t stackSize /*= 12 * 1024*/) noexcept : Super(interval, stackSize) {
	if(ApplicationInstance == nullptr){
		ApplicationInstance = this;
	}
}

Application::~Application() noexcept {
	if(ApplicationInstance == this){
		ApplicationInstance = nullptr;
	}
}

void Application::registerLifetimeObject(Object* object) noexcept{
	lifetimeObjects.insert(object);
}

void Application::postInitProperties() noexcept {
	Super::postInitProperties();

	eventScanner = newObject<EventScanner>();
}

void Application::begin() noexcept {
	Super::begin();
}

void Application::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);
}

void Application::end(EndReason reason) noexcept {
	// TODO right now only Quit reason is supported, this needs to be expanded in the future
	reason = EndReason::Quit;
	shuttingDown = true;

	Super::end(reason);

	ObjectManager::get()->forEachObject([](Object* object) {
		if(AsyncEntity* entity = cast<AsyncEntity>(object)){
			entity->destroy();
		}

		return false;
	});
}

SubclassOf<GarbageCollector> Application::getGarbageCollectorClass() const noexcept{
	return GarbageCollector::staticClass();
}

TickType_t Application::getEventScanningTime() const noexcept{
	return 0;
}

TickType_t Application::getTickingInterval() const noexcept{
	return 0;
}
