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