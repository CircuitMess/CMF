#ifndef CMF_ENUM_H
#define CMF_ENUM_H

#include <concepts>
#include <cstdio>
#include <numeric>

template<typename T = int, typename = std::enable_if<std::integral<T>, T>::type>
class Enum {
public:
	using Type = T;

	inline constexpr Enum() noexcept : value(T()) {}

	template<typename E>
	inline constexpr Enum(E val) noexcept : value(static_cast<T>(val)) {}

	inline virtual constexpr bool hasNone() const noexcept {
		return false;
	}

	inline virtual constexpr T none() const noexcept {
		return T();
	}

	inline virtual constexpr bool hasCount() const noexcept {
		return false;
	}

	inline virtual constexpr size_t count() const noexcept {
		return std::numeric_limits<T>().max();
	}

	inline virtual constexpr operator T() const noexcept final {
		return value;
	}

private:
	T value;
};

#define DECLARE_CLASS_ENUM(Name, Type, ...) 																						\
	class Name : public Enum<Type> {           																						\
	public:                                																							\
		inline constexpr Name() noexcept : Enum() {} 																				\
                                            																						\
    	template<typename E>                   																						\
		inline constexpr Name(E val) noexcept : Enum(val) {} 																		\
                                            																						\
		enum {                                    																					\
        	None,                              																						\
			__VA_ARGS__ __VA_OPT__(,)                																				\
			COUNT																													\
		};                                        																					\
																																	\
		inline virtual constexpr bool hasNone() const noexcept override { return true; } 											\
                                            																						\
		inline virtual constexpr Type none() const noexcept override { return static_cast<Type>(Name::None); } 						\
                                                         																			\
		inline virtual constexpr bool hasCount() const noexcept override { return true; }											\
                                            																						\
		inline virtual constexpr size_t count() const noexcept override { return static_cast<size_t>(Name::COUNT); }				\
	}

#define DECLARE_CLASS_ENUM_EXACT(Name, Type, ...) 																					\
	class Name : public Enum<Type> {           																						\
	public:                                																							\
		inline constexpr Name() noexcept : Enum() {} 																				\
                                            																						\
    	template<typename E>                   																						\
		inline constexpr Name(E val) noexcept : Enum(val) {} 																		\
                                            																						\
		enum {                                 																						\
			__VA_ARGS__																												\
		};                       																									\
	}

#define DECLARE_ENUM(Name, ...) DECLARE_CLASS_ENUM(Name, int, __VA_ARGS__)

#define DECLARE_ENUM_EXACT(Name, ...) DECLARE_CLASS_ENUM_EXACT(Name, int, __VA_ARGS__)

#endif //CMF_ENUM_H