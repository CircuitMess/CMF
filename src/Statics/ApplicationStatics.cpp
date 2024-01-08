#include "ApplicationStatics.h"
#include "Core/Application.h"

Application* ApplicationStatics::getApplication() noexcept {
	return Application::s_ApplicationInstance;
}
