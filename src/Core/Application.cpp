#include "Application.h"

Application::Application() noexcept : Super() {
	if(s_ApplicationInstance == nullptr){
		s_ApplicationInstance = this;
	}
}

Application::~Application() noexcept {
	if(s_ApplicationInstance == this){
		s_ApplicationInstance = nullptr;
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