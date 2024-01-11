#ifndef CMF_STRONGOBJECTPTR_H
#define CMF_STRONGOBJECTPTR_H

#include "ObjectPtr.h"

template<typename T>
class StrongObjectPtr : public ObjectPtr<T, true> {
public:
	inline constexpr StrongObjectPtr() noexcept = default;

	inline constexpr StrongObjectPtr(const StrongObjectPtr& other) noexcept : ObjectPtr<T, true>(other) {}

	inline constexpr StrongObjectPtr(StrongObjectPtr&& other) noexcept : ObjectPtr<T, true>(std::move(other)) {}

	inline constexpr StrongObjectPtr(const ObjectPtr<T, false>& other) noexcept : ObjectPtr<T, true>(other) {}

	inline constexpr StrongObjectPtr(ObjectPtr<T, false>&& other) noexcept : ObjectPtr<T, true>(std::move(other)) {}

	inline constexpr StrongObjectPtr(Object* object) noexcept : ObjectPtr<T, true>(object) {}

	inline constexpr StrongObjectPtr(nullptr_t) noexcept : ObjectPtr<T, true>(nullptr) {}

	virtual ~StrongObjectPtr() noexcept override = default;

	inline constexpr StrongObjectPtr& operator = (StrongObjectPtr&& other) noexcept {
		ObjectPtr<T, true>::operator=(std::move(other));

		return *this;
	}

	template<typename _T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
	inline constexpr StrongObjectPtr& operator = (const StrongObjectPtr<_T>& other) noexcept {
		ObjectPtr<T, true>::template operator=<_T>(other);

		return *this;
	}

	template<typename _T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
	inline constexpr StrongObjectPtr& operator = (StrongObjectPtr<_T>&& other) noexcept {
		ObjectPtr<T, true>::template operator=<_T>(std::move(other));

		return *this;
	}
};

#endif //CMF_STRONGOBJECTPTR_H