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

template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
static void startCMF(){
	// TODO: init app, start ticking loop

	T* app = new T();
	//app->begin(); TODO: doesnt work for now, will work when additional object memory management is done
}

#define CMF_MAIN(AppName) 				\
	extern "C" void app_main() 			\
	{ 									\
		startCMF<AppName>(); 			\
	}									\

#endif //CMF_ENTRYPOINT_H
