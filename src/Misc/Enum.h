#ifndef CMF_ENUM_H
#define CMF_ENUM_H

#include <concepts>
#include <cstdio>
#include <numeric>

template<typename T = int, typename = std::enable_if<std::integral<T>, T>::type>
class Enum {
public:
	inline constexpr Enum() noexcept : value(T()) {}

	template<typename E>
	inline constexpr Enum(E val) noexcept : value(static_cast<T>(val)) {}

	inline virtual constexpr std::vector<T> getValues() const noexcept{
		return {};
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
			__VA_ARGS__																												\
		};                                        																					\
																																	\
		inline virtual constexpr std::vector<Type> getValues() const noexcept override { return {__VA_ARGS__}; } 					\
	}

#define DECLARE_CLASS_ENUM_STARTING_WITH(Name, Type, First, ...) 																	\
	class Name : public Enum<Type> {           																						\
	public:                                																							\
		inline constexpr Name() noexcept : Enum() {} 																				\
                                            																						\
    	template<typename E>                   																						\
		inline constexpr Name(E val) noexcept : Enum(val) {} 																		\
                                            																						\
		enum {                                                                          											\
			__first = First - 1,																									\
			__VA_ARGS__																												\
		};                                        																					\
																																	\
		inline virtual constexpr std::vector<Type> getValues() const noexcept override { return {__VA_ARGS__}; } 					\
	}

#define DECLARE_ENUM(Name, ...) DECLARE_CLASS_ENUM(Name, int, __VA_ARGS__)

#define DECLARE_ENUM_STARTING_WITH(Name, First, ...) DECLARE_CLASS_ENUM_STARTING_WITH(Name, int, First, __VA_ARGS__)

#endif //CMF_ENUM_H