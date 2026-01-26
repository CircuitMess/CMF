#include "Application.h"

Application::Application(TickType_t interval /*= 0*/, size_t stackSize /*= 12 * 1024*/, uint8_t threadPriority /*= 5*/, int8_t cpuCore /*= -1*/) noexcept : Super(interval, stackSize, threadPriority, cpuCore) {
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

void Application::__postInitProperties() noexcept {
	eventScanner = newObject<EventScanner>();

	Super::__postInitProperties();
}

void Application::__end(EndReason reason) noexcept {
	// TODO right now only Quit reason is supported, this needs to be expanded in the future
	reason = EndReason::Quit;
	shuttingDown = true;

	Super::__end(reason);

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