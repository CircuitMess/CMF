#ifndef CMF_ENTRYPOINT_H
#define CMF_ENTRYPOINT_H

#include "Core.h"
#include "Misc/Djb.h"
#include "Object/Object.h"
#include "Memory/Cast.h"

template<typename T>
static void CMFAppStart(){
	ExampleDerivedObject* object = new ExampleDerivedObject();

	printf("Derived is base: %d\n", ExampleDerivedObject::Implements<Object>());

	printf("Derived is interface 1: %d\n", ExampleDerivedObject::Implements<Interface1>());
	printf("Derived is interface 2: %d\n", ExampleDerivedObject::Implements<Interface2>());

	Object* pureObject = object;

	printf("Pure object is derived: %d\n", pureObject->IsA(ExampleDerivedObject::StaticClass()));

	printf("Pure object derives pure object: %d\n", Object::Implements<Object>());
	printf("Pure object derives interface 1: %d\n", Object::Implements<Interface1>());

	printf("Derived is pure: %d\n", object->IsA(Object::StaticClass()));

	printf("Test derives wrong template of right interface: %d\n", TestTemplateObject::Implements<Interface3<float>>());
	printf("Test derives right template of right interface: %d\n", TestTemplateObject::Implements<Interface3<int>>());

	printf("Good cast: %p\n", Cast<ExampleDerivedObject>(pureObject));
	printf("Bad cast: %p\n", Cast<TestTemplateObject>(pureObject));

	printf("Done\n");

	// TODO: init app, start ticking loop
}

#define CMF_MAIN(AppName) \
	extern "C" void app_main() \
	{ \
		CMFAppStart<AppName>(); \
	}
#else

#endif //CMF_ENTRYPOINT_H
