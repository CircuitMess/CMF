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

template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
static void startCMF(){
	ExampleDerivedObject* object = new ExampleDerivedObject();

	printf("Derived is base: %d\n", ExampleDerivedObject::implements<Object>());

	printf("Derived is interface 1: %d\n", ExampleDerivedObject::implements<Interface1>());
	printf("Derived is interface 2: %d\n", ExampleDerivedObject::implements<Interface2>());

	Object* pureObject = object;

	printf("Pure object is derived: %d\n", pureObject->isA(ExampleDerivedObject::staticClass()));

	printf("Test derives wrong template of right interface: %d\n", TestTemplateObject::implements<Interface3<float>>());
	printf("Test derives right template of right interface: %d\n", TestTemplateObject::implements<Interface3<int>>());

	printf("Good cast: %p\n", cast<ExampleDerivedObject>(pureObject));
	printf("Bad cast: %p\n", cast<TestTemplateObject>(pureObject));

	Interface<Interface1> testInterface(object);
	printf("Good interface object: %p, interface: %p\n", testInterface.getObject(), testInterface.getInterface());

	Interface<Interface1> secondTestInterface(new ThirdExampleDerivedObject());
	printf("Bad interface object: %p, interface: %p\n", secondTestInterface.getObject(), secondTestInterface.getInterface());

	printf("Done\n");

	// TODO: init app, start ticking loop
}

#define CMF_MAIN(AppName) \
	extern "C" void app_main() \
	{ \
		startCMF<AppName>(); \
	}

#endif //CMF_ENTRYPOINT_H
