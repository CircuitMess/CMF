#ifndef CMF_OBJECTPTR_H
#define CMF_OBJECTPTR_H

#include <concepts>
#include <type_traits>
#include "Object/Object.h"
#include "Memory/ObjectManager.h"

// Spec: pointers are held to a object in memory as long as it exists in memory. After the object is destroyed, pointer will stop being valid but will still point to the
// memory of the object until the object is deleted from memory, in which case, the pointer will change to nullptr

template<typename T, bool KeepAlive, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
class ObjectPtr : public std::integral_constant<bool, KeepAlive> {
public:
	inline ObjectPtr() noexcept = default;

	inline ObjectPtr(const ObjectPtr& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	inline ObjectPtr(ObjectPtr&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
		other = nullptr;
	}

	explicit inline ObjectPtr(const ObjectPtr<T, !KeepAlive>& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	explicit inline ObjectPtr(ObjectPtr<T, !KeepAlive>&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
		other = nullptr;
	}

	explicit inline ObjectPtr(Object* object) noexcept {
		ptr = object;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	explicit inline ObjectPtr(nullptr_t) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = nullptr;
	}

	virtual ~ObjectPtr() noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = nullptr;
	}

	inline ObjectPtr& operator = (const ObjectPtr& other) noexcept {
		if(&other == this){
			return *this;
		}

		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		return *this;
	}

	inline ObjectPtr& operator = (ObjectPtr&& other) noexcept {
		if(&other == this){
			return *this;
		}

		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		other = nullptr;

		return *this;
	}

	inline ObjectPtr& operator = (const ObjectPtr<T, !KeepAlive>& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = &other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		return *this;
	}

	inline ObjectPtr& operator = (ObjectPtr<T, !KeepAlive>&& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = &other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		other = nullptr;

		return *this;
	}

	inline ObjectPtr& operator = (Object* object) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = object;
		ObjectManager::get()->registerReference(ptr, (*this)());

		return *this;
	}

	inline ObjectPtr& operator = (nullptr_t) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = nullptr;

		return *this;
	}

	inline T* operator * () const noexcept {
		return get();
	}

	inline T* operator -> () const noexcept {
		return get();
	}

	inline explicit operator bool() const noexcept {
		return isValid();
	}

	inline T* get() const noexcept {
		return ptr;
	}

	inline bool isValid() const noexcept {
		if(ptr == nullptr){
			return false;
		}

		return ObjectManager::get()->isValid(ptr);
	}

private:
	T* ptr;
};

#endif //CMF_OBJECTPTR_H