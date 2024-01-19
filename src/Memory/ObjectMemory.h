#ifndef CMF_OBJECTMEMORY_H
#define CMF_OBJECTMEMORY_H

#include <concepts>
#include "Cast.h"
#include "Object/Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Statics/ApplicationStatics.h"
#include "Core/Application.h"

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Object* owner = nullptr, Args&&... args) noexcept {
	StrongObjectPtr<T> newObject = new T(args...);

	if(Object* object = cast<Object>(newObject.get())){
		if(owner != nullptr){
			object->setOwner(owner);
		}else{
			object->setOwner(ApplicationStatics::getApplication());
		}

		object->postInitProperties();
		object->onCreated();
	}

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

	if(Object* object = cast<Object>(*newObjectPtr)){
		object->postInitProperties();
		object->onCreated();
	}

	if(owner != nullptr){
		newObjectPtr->setOwner(owner);
	}else{
		newObjectPtr->setOwner(ApplicationStatics::getApplication());
	}

	return newObjectPtr;
}

#endif //CMF_OBJECTMEMORY_H