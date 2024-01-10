#ifndef CMF_OBJECTMEMORY_H
#define CMF_OBJECTMEMORY_H

#include <concepts>
#include "Cast.h"
#include "Object/Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Args&&... args) noexcept {
	StrongObjectPtr<T> newObject = new T(args...);

	if(Object* object = cast<Object>(newObject.get())){
		object->postInitProperties();
	}

	return newObject;
}

template<typename T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<Object> newObject(Class* cls) noexcept {
	if(cls == nullptr){
		return nullptr;
	}

	Object* newObject = cls->createDefaultObject();

	if(newObject == nullptr){
		return nullptr;
	}

	StrongObjectPtr<T> newObjectPtr = cast<T>(newObject);
	if(!newObjectPtr.isValid()){
		return nullptr;
	}

	newObjectPtr->postInitProperties();

	return newObjectPtr;
}

#endif //CMF_OBJECTMEMORY_H