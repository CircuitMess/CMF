#ifndef CMF_ENTRYPOINT_H
#define CMF_ENTRYPOINT_H

#include "Core.h"
#include "Misc/Djb.h"
#include "Object/Object.h"

template<typename T>
static T* CMFAppStart(){
	ExampleDerivedObject* object = new ExampleDerivedObject();

	printf("Derived is base: %d\n", ExampleDerivedObject::Derives<Object>());

	printf("Derived is interface 1: %d\n", ExampleDerivedObject::Derives<Interface1>());
	printf("Derived is interface 2: %d\n", ExampleDerivedObject::Derives<Interface2>());

	Object* pureObject = object;

	printf("Pure object is derived: %d\n", pureObject->IsA(ExampleDerivedObject::StaticClass()));

	printf("Pure object derives pure object: %d\n", Object::Derives<Object>());
	printf("Pure object derives interface 1: %d\n", Object::Derives<Interface1>());

	printf("Derived is pure: %d\n", object->IsA(Object::StaticClass()));

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
