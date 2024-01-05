#include "Application.h"

Application::Application() noexcept : Super() {

}

Application::~Application() noexcept {

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