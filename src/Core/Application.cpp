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

void Application::PostInitProperties() noexcept {
	Super::PostInitProperties();
}

void Application::Begin() noexcept {
	Super::Begin();
}

void Application::Tick(float deltaTime) noexcept {
	Super::Tick(deltaTime);
}

void Application::End() noexcept {
	Super::End();
}