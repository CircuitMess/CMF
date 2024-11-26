#ifndef CMF_OBJECTPTR_H
#define CMF_OBJECTPTR_H

#include <concepts>
#include <type_traits>
#include "Memory/Cast.h"
#include "Object/Object.h"
#include "Memory/ObjectManager.h"

template<typename T, bool KeepAlive>
class ObjectPtr : public std::integral_constant<bool, KeepAlive> {
public:
	inline constexpr ObjectPtr() noexcept = default;

	inline constexpr ObjectPtr(const ObjectPtr& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
	}

	inline constexpr ObjectPtr(ObjectPtr&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
		other = nullptr;
	}

	inline constexpr ObjectPtr(const ObjectPtr<T, !KeepAlive>& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
	}

	inline constexpr ObjectPtr(ObjectPtr<T, !KeepAlive>&& other) noexcept {
		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
		other = nullptr;
	}

	template<typename _T, bool _KeepAlive>
	inline constexpr ObjectPtr(const ObjectPtr<_T, _KeepAlive>& other) noexcept {
		if(cast<T>(*other) == nullptr){
			return;
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
	}

	template<typename _T, bool _KeepAlive>
	inline constexpr ObjectPtr(ObjectPtr<_T, _KeepAlive>&& other) noexcept {
		if(cast<T>(*other) == nullptr){
			return;
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());
		other = nullptr;
	}

	inline constexpr ObjectPtr(Object* object) noexcept {
		ptr = object;
		ObjectManager::get()->registerReference(&ptr, (*this)());
	}

	inline constexpr ObjectPtr(nullptr_t) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = nullptr;
	}

	virtual ~ObjectPtr() noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = nullptr;
	}

	inline constexpr ObjectPtr& operator = (const ObjectPtr& other) noexcept {
		if(&other == this){
			return *this;
		}

		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		return *this;
	}

	inline constexpr ObjectPtr& operator = (ObjectPtr&& other) noexcept {
		if(&other == this){
			return *this;
		}

		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		other = nullptr;

		return *this;
	}

	inline constexpr ObjectPtr& operator = (const ObjectPtr<T, !KeepAlive>& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		return *this;
	}

	inline constexpr ObjectPtr& operator = (ObjectPtr<T, !KeepAlive>&& other) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		other = nullptr;

		return *this;
	}

	template<typename _T, bool _KeepAlive>
	inline constexpr ObjectPtr& operator = (const ObjectPtr<_T, _KeepAlive>& other) noexcept {
		if(cast<T>(*other) == nullptr){
			return *this;
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		return *this;
	}

	template<typename _T, bool _KeepAlive>
	inline constexpr ObjectPtr& operator = (ObjectPtr<_T, _KeepAlive>&& other) noexcept {
		if(cast<T>(*other) == nullptr){
			return *this;
		}

		ptr = *other;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		other = nullptr;

		return *this;
	}

	inline constexpr ObjectPtr& operator = (Object* object) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = object;
		ObjectManager::get()->registerReference(&ptr, (*this)());

		return *this;
	}

	inline constexpr ObjectPtr& operator = (nullptr_t) noexcept {
		if(ptr != nullptr){
			ObjectManager::get()->unregisterReference(&ptr, (*this)());
		}

		ptr = nullptr;

		return *this;
	}

	inline constexpr bool operator == (const ObjectPtr& other) noexcept {
		if(!other.isValid()){
			return !isValid();
		}

		return ptr == other.ptr;
	}

	inline constexpr bool operator == (const ObjectPtr<T, !KeepAlive>& other) noexcept {
		if(!other.isValid()){
			return !isValid();
		}

		return ptr == other.ptr;
	}

	inline constexpr bool operator == (const Object* object) noexcept {
		if(object == nullptr){
			return !isValid();
		}

		return ptr == object;
	}

	inline constexpr bool operator == (nullptr_t) noexcept {
		return isValid();
	}

	inline constexpr bool operator > (const ObjectPtr&& other) const noexcept {
		return ptr > *other;
	}

	inline constexpr bool operator > (const ObjectPtr<T, !KeepAlive>& other) const noexcept {
		return ptr > *other;
	}

	inline constexpr bool operator > (const Object* object) const noexcept {
		return ptr > object;
	}

	inline constexpr bool operator < (const ObjectPtr&& other) const noexcept {
		return ptr < *other;
	}

	inline constexpr bool operator < (const ObjectPtr<T, !KeepAlive>& other) const noexcept {
		return ptr < *other;
	}

	inline constexpr bool operator < (const Object* object) const noexcept {
		return ptr < object;
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
		return cast<T>(ptr);
	}

	inline constexpr bool isValid() const noexcept {
		if(get() == nullptr){
			return false;
		}

		return ObjectManager::get()->isValid(ptr);
	}

private:
	Object* ptr = nullptr;
};

#endif //CMF_OBJECTPTR_H