#ifndef CMF_WEAKOBJECTPTR_H
#define CMF_WEAKOBJECTPTR_H

#include "ObjectPtr.h"

template<typename T>
class WeakObjectPtr : public ObjectPtr<T, false> {
public:
	inline constexpr WeakObjectPtr() noexcept = default;

	inline constexpr WeakObjectPtr(const WeakObjectPtr& other) noexcept : ObjectPtr<T, false>(other) {}

	inline constexpr WeakObjectPtr(WeakObjectPtr&& other) noexcept : ObjectPtr<T, false>(std::move(other)) {}

	inline constexpr WeakObjectPtr(const ObjectPtr<T, true>& other) noexcept : ObjectPtr<T, false>(other) {}

	inline constexpr WeakObjectPtr(ObjectPtr<T, true>&& other) noexcept : ObjectPtr<T, false>(std::move(other)) {}

	inline constexpr WeakObjectPtr(Object* object) noexcept : ObjectPtr<T, false>(object) {}

	inline constexpr WeakObjectPtr(nullptr_t) noexcept : ObjectPtr<T, false>(nullptr) {}

	virtual ~WeakObjectPtr() noexcept = default;

	inline constexpr WeakObjectPtr& operator = (WeakObjectPtr&& other) noexcept {
		ObjectPtr<T, false>::operator=(std::move(other));

		return *this;
	}

	template<typename _T>
	inline constexpr WeakObjectPtr& operator = (const WeakObjectPtr<_T>& other) noexcept {
		ObjectPtr<T, false>::template operator=<_T>(other);

		return *this;
	}

	template<typename _T>
	inline constexpr WeakObjectPtr& operator = (WeakObjectPtr<_T>&& other) noexcept {
		ObjectPtr<T, false>::template operator=<_T>(std::move(other));

		return *this;
	}
};

#endif //CMF_WEAKOBJECTPTR_H
