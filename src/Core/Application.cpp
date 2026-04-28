#include "Application.h"

Application::Application(TickType_t interval /*= CONFIG_CMF_APPLICATION_TICK_INTERVAL*/, size_t stackSize /*= CONFIG_CMF_APPLICATION_STACK_SIZE*/,
	uint8_t threadPriority /*= CONFIG_CMF_APPLICATION_THREAD_PRIORITY*/, int8_t cpuCore /*= CONFIG_CMF_APPLICATION_CPU_CORE*/) noexcept :
		Super(interval, stackSize, threadPriority, cpuCore) {
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
		CMF_LOG(CMF, LogLevel::Warning, "Application destroyed without an ObjectManager available, skipping cleanup");
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