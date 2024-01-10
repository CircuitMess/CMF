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
	inline constexpr ObjectPtr() noexcept = default;

	inline constexpr ObjectPtr(const ObjectPtr& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	inline constexpr ObjectPtr(ObjectPtr&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
		other = nullptr;
	}

	explicit inline constexpr ObjectPtr(const ObjectPtr<T, !KeepAlive>& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	explicit inline constexpr ObjectPtr(ObjectPtr<T, !KeepAlive>&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(ptr, (*this)());
		other = nullptr;
	}

	explicit inline constexpr ObjectPtr(Object* object) noexcept {
		ptr = object;
		ObjectManager::get()->registerReference(ptr, (*this)());
	}

	explicit inline constexpr ObjectPtr(nullptr_t) noexcept {
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

	inline constexpr ObjectPtr& operator = (const ObjectPtr& other) noexcept {
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

	inline constexpr ObjectPtr& operator = (ObjectPtr&& other) noexcept {
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

	inline constexpr ObjectPtr& operator = (const ObjectPtr<T, !KeepAlive>& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = &other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		return *this;
	}

	inline constexpr ObjectPtr& operator = (ObjectPtr<T, !KeepAlive>&& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = &other;
		ObjectManager::get()->registerReference(ptr, (*this)());

		other = nullptr;

		return *this;
	}

	inline constexpr ObjectPtr& operator = (Object* object) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = object;
		ObjectManager::get()->registerReference(ptr, (*this)());

		return *this;
	}

	inline constexpr ObjectPtr& operator = (nullptr_t) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(ptr, (*this)());
		}

		ptr = nullptr;

		return *this;
	}

	inline constexpr T* operator * () const noexcept {
		return get();
	}

	inline constexpr T* operator -> () const noexcept {
		return get();
	}

	inline constexpr explicit operator bool() const noexcept {
		return isValid();
	}

	inline constexpr T* get() const noexcept {
		return ptr;
	}

	inline constexpr bool isValid() const noexcept {
		if(ptr == nullptr){
			return false;
		}

		return ObjectManager::get()->isValid(ptr);
	}

private:
	T* ptr = nullptr;
};

#endif //CMF_OBJECTPTR_H