#ifndef CMF_ENTRYPOINT_H
#define CMF_ENTRYPOINT_H

#include "Core.h"

template<typename T>
static T* CMFAppStart(){
	// TODO: init app, start ticking loop
	return nullptr;
}

#define CMF_MAIN(AppName) \
	extern "C" void app_main() \
	{ \
		CMFAppStart<AppName>(); \
	}
#else

#endif //CMF_ENTRYPOINT_H
