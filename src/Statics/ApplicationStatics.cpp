#include "ApplicationStatics.h"
#include "Core/Application.h"

Application* ApplicationStatics::GetApplication() noexcept {
	return Application::s_ApplicationInstance;
}
