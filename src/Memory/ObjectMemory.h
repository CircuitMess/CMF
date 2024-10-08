#ifndef CMF_OBJECTMEMORY_H
#define CMF_OBJECTMEMORY_H

#include <concepts>
#include "Cast.h"
#include "Object/Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Statics/ApplicationStatics.h"
#include "ObjectManager.h"
#include "Object/Interface.h"

void initObject(Object* object, Object* owner) noexcept;

bool isValid(const Object* object) noexcept;

template<typename T, bool KeepAlive>
inline bool isValid(const ObjectPtr<T, KeepAlive>& object) noexcept {
	if(object == nullptr){
		return false;
	}

	return ObjectManager::get()->isValid(object.get());
}

template<typename T>
inline bool isValid(const Interface<T>& interface) noexcept {
	if(!interface){
		return false;
	}

	return isValid(interface.getObject());
}

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Object* owner = nullptr, Args&&... args) noexcept {
	StrongObjectPtr<T> newObject = new T(args...);

	initObject(cast<Object>(newObject.get()), owner);

	return newObject;
}

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Object* owner = nullptr, Args&&... args) noexcept requires (sizeof...(Args) == 0){
	return newObject<T>(T::staticClass());
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

template<typename T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> objectFromByteArray(const std::vector<uint8_t>& data, Object* owner = nullptr) noexcept {
	OutArchive archive = data;
	uint32_t classID = 0;
	archive << classID;

	StrongObjectPtr<T> object = newObject<T>(Class::getClasByID(classID), owner);
	if(!object.isValid()){
		return nullptr;
	}

	object->serialize(archive);
}

inline void byteArrayFromObject(Object* object, std::vector<uint8_t>& data) noexcept {
	if(object == nullptr){
		return;
	}

	InArchive archive;
	archive << object->getStaticClass()->getID();

	object->serialize(archive);

	archive.toByteArray(data);
}

#endif //CMF_OBJECTMEMORY_H