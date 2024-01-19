#ifndef CMF_OBJECTMEMORY_H
#define CMF_OBJECTMEMORY_H

#include <concepts>
#include "Cast.h"
#include "Object/Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Statics/ApplicationStatics.h"

void initObject(Object* object, Object* owner) noexcept;

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Object* owner = nullptr, Args&&... args) noexcept {
	StrongObjectPtr<T> newObject = new T(args...);

	initObject(cast<Object>(newObject.get()), owner);

	return newObject;
}

template<typename T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(const Class* cls, Object* owner = nullptr) noexcept {
	if(cls == nullptr){
		return nullptr;
	}

	StrongObjectPtr<T> newObjectPtr = cast<T>(cls->createDefaultObject());
	if(!newObjectPtr.isValid()){
		return nullptr;
	}

	initObject(cast<Object>(newObjectPtr.get()), owner);

	return newObjectPtr;
}

#endif //CMF_OBJECTMEMORY_H