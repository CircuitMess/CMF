#ifndef CMF_ENTRYPOINT_H
#define CMF_ENTRYPOINT_H

#include <cstdio>
#include <concepts>
#include "Core.h"
#include "Misc/Djb.h"
#include "Object/Object.h"
#include "Memory/Cast.h"
#include "Object/Interface.h"
#include "Application.h"
#include "Memory/Cast.h"
#include "Containers/BinaryTree.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Memory/ObjectMemory.h"
#include "Memory/GarbageCollector.h"
#include "Containers/Queue.h"
#include "Event/EventHandle.h"
#include "Event/EventDelegate.h"
#include "Event/EventBroadcaster.h"
#include "Log/Log.h"

class CMF {
public:
	template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
	static void start(){
		App = newObject<T>();
		if(!App.isValid()){
			CMF_LOG(LogCMF, Error, "Application instance could not be created.");
			return;
		}

		SubclassOf<GarbageCollector> garbageCollectorClass = App->getGarbageCollectorClass();
		if(garbageCollectorClass == nullptr){
			TrashCollector = newObject<GarbageCollector>();
		}else{
			TrashCollector = newObject<GarbageCollector>(*garbageCollectorClass);
		}

		if(!TrashCollector.isValid()){
			CMF_LOG(LogCMF, Error, "GarbageCollector instance could not be created.");
		}
	}

private:
	inline static StrongObjectPtr<Application> App = nullptr;
	inline static StrongObjectPtr<GarbageCollector> TrashCollector = nullptr;
};

#define CMF_MAIN(AppName) 				\
	extern "C" void app_main() 			\
	{ 									\
		CMF::start<AppName>();      	\
		vTaskDelete(nullptr);			\
	}									\

#endif //CMF_ENTRYPOINT_H
