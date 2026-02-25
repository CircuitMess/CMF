#include "Application.h"

Application::Application(TickType_t interval /*= 0*/, size_t stackSize /*= 12 * 1024*/, uint8_t threadPriority /*= 5*/, int8_t cpuCore /*= -1*/) noexcept : Super(interval, stackSize, threadPriority, cpuCore) {
	if(ApplicationInstance == nullptr){
		ApplicationInstance = this;
	}

	eventScanner = newObject<EventScanner>();
}

Application::~Application() noexcept {
	if(ApplicationInstance == this){
		ApplicationInstance = nullptr;
	}else{
		return;
	}

	ObjectManager* manager = ObjectManager::get();
	if(manager == nullptr){
		return;
	}

	manager->forEachObject([this](Object* object) {
		if(object == this) {
			return false;
		}

		if(AsyncEntity* asyncEntity = cast<AsyncEntity>(object)){
			delete asyncEntity;
		}

		return false;
	});
}

void Application::registerLifetimeObject(Object* object) noexcept{
	lifetimeObjects.insert(object);
}

SubclassOf<GarbageCollector> Application::getGarbageCollectorClass() const noexcept{
	return GarbageCollector::staticClass();
}