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

class CMF {
public:
	template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
	static void start(){
		// TODO: init app, start ticking loop


		// TODO: this causes type error with smart pointers, this should be addressed further and fixed so that assignment of different types silently casts the types in the background (if possible)
		CMF::App = newObject<T>();
	}

private:
	inline static StrongObjectPtr<Application> App = nullptr;
	inline static StrongObjectPtr<GarbageCollector> TrashCollector = newObject<::GarbageCollector>();
};

#define CMF_MAIN(AppName) 				\
	extern "C" void app_main() 			\
	{ 									\
		CMF::start<AppName>(); 			\
	}									\

#endif //CMF_ENTRYPOINT_H
