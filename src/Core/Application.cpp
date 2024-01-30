#include "Application.h"

Application::Application() noexcept : Super() {
	if(ApplicationInstance == nullptr){
		ApplicationInstance = this;
	}
}

Application::~Application() noexcept {
	if(ApplicationInstance == this){
		ApplicationInstance = nullptr;
	}
}

void Application::postInitProperties() noexcept {
	Super::postInitProperties();
}

void Application::begin() noexcept {
	Super::begin();
}

void Application::tick(float deltaTime) noexcept {
	Super::tick(deltaTime);
}

void Application::end() noexcept {
	Super::end();
}

SubclassOf<GarbageCollector> Application::getGarbageCollectorClass() const noexcept{
	return GarbageCollector::staticClass();
}

void Application::onOwnerChanged(Object* oldOwner) noexcept{
	Super::onOwnerChanged(oldOwner);

	// This is on purpose, the application should not have an owner to try to prevent accidental circular ownership
	if(getOwner() != nullptr){
		setOwner(nullptr);
	}
}

TickType_t Application::getEventScanningTime() const noexcept{
	return 0;
}

TickType_t Application::getTickingInterval() const noexcept{
	return 0;
}
