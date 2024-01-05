#ifndef CMF_ENTRYPOINT_H
#define CMF_ENTRYPOINT_H

#include "Core.h"
#include "Misc/Djb.h"
#include "Object/Object.h"
#include "Memory/Cast.h"
#include "Object/Interface.h"
#include "Application.h"

template<typename T, typename = std::enable_if<std::derived_from<T, Application>>::type>
static void CMFAppStart(){
	ExampleDerivedObject* object = new ExampleDerivedObject();

	printf("Derived is base: %d\n", ExampleDerivedObject::Implements<Object>());

	printf("Derived is interface 1: %d\n", ExampleDerivedObject::Implements<Interface1>());
	printf("Derived is interface 2: %d\n", ExampleDerivedObject::Implements<Interface2>());

	Object* pureObject = object;

	printf("Pure object is derived: %d\n", pureObject->IsA(ExampleDerivedObject::StaticClass()));

	printf("Test derives wrong template of right interface: %d\n", TestTemplateObject::Implements<Interface3<float>>());
	printf("Test derives right template of right interface: %d\n", TestTemplateObject::Implements<Interface3<int>>());

	printf("Good cast: %p\n", Cast<ExampleDerivedObject>(pureObject));
	printf("Bad cast: %p\n", Cast<TestTemplateObject>(pureObject));

	Interface<Interface1> testInterface(object);
	printf("Good interface object: %p, interface: %p\n", testInterface.GetObject(), testInterface.GetInterface());

	Interface<Interface1> secondTestInterface(new ThirdExampleDerivedObject());
	printf("Bad interface object: %p, interface: %p\n", secondTestInterface.GetObject(), secondTestInterface.GetInterface());

	printf("Done\n");

	// TODO: init app, start ticking loop
}

#define CMF_MAIN(AppName) \
	extern "C" void app_main() \
	{ \
		CMFAppStart<AppName>(); \
	}

#endif //CMF_ENTRYPOINT_H
