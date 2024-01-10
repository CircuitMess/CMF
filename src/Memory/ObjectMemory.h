#ifndef CMF_OBJECTMEMORY_H
#define CMF_OBJECTMEMORY_H

#include <concepts>
#include "Object/Object.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"

template<typename T, typename ...Args, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
inline StrongObjectPtr<T> newObject(Args&&... args) noexcept {
	return StrongObjectPtr<T>(new T(args...));
}

#endif //CMF_OBJECTMEMORY_H