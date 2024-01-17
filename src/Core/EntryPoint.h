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

class TestObject : public Object {
	GENERATED_BODY(TestObject, Object)

public:
	void test() {
		EventHandle<int, float, void*> testHandle;

		testHandle.bind(this, &TestObject::testHandle);

		testHandle.scan(0);
	}

	void testHandle(int i, float f, void* v) {

	}
};

class CMF {
public:
	template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
	static void start(){
		// TODO: init app, start ticking loop

		const Queue<int> testQ;
		int i;
		testQ.front(i);

		TrashCollector = newObject<GarbageCollector>();
		App = newObject<T>();
	}

private:
	inline static StrongObjectPtr<Application> App = nullptr;
	inline static StrongObjectPtr<GarbageCollector> TrashCollector = nullptr;
};

#define CMF_MAIN(AppName) 				\
	extern "C" void app_main() 			\
	{ 									\
		CMF::start<AppName>(); 			\
	}									\

#endif //CMF_ENTRYPOINT_H
