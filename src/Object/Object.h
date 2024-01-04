#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <type_traits>
#include "Class.h"
#include "Misc/Djb.h"

class Object {
public:
	inline static const Class* StaticClass() noexcept {
		return staticClass;
	}

	inline virtual const Class* GetStaticClass() const noexcept {
		return StaticClass();
	}

	inline virtual bool IsA(const Class* other) const noexcept {
		return other->GetID() == StaticClass()->GetID();
	}

	template<typename T>
	inline bool IsA() const noexcept {
		return IsA(T::StaticClass());
	}

	template<typename T>
	static inline constexpr bool Derives() noexcept {
		return false;
	}

private:
	using ClassType = Class;
	inline static const ClassType* staticClass = new ClassType(STRING_HASH("Object"));
};

#define GENERATED_BODY(ObjectName, SuperObject, ...) 																		\
	static_assert(std::derived_from<SuperObject, Object>, "Object must have and inherit a base Object class.");				\
																															\
private:																													\
	using Super = SuperObject;																								\
																															\
public:																														\
	inline static const Class* StaticClass() noexcept {																		\
		return staticClass;																									\
	}																														\
																															\
	inline virtual const Class* GetStaticClass() const noexcept override {													\
		return StaticClass();																								\
	}																														\
																															\
	inline virtual bool IsA(const Class* other) const noexcept override {													\
		return other->GetID() == StaticClass()->GetID() || Super::IsA(other);												\
	}																														\
																															\
	template<typename T>																									\
	inline bool IsA() const noexcept {																						\
		return IsA(T::StaticClass());																						\
	}																														\
																															\
	template<typename T>																									\
	inline static constexpr bool Derives() noexcept {																		\
		return staticClass->Derives<T>() || Super::Derives<T>();															\
	}                                                      																	\
																															\
private:																													\
	template<typename... Types>																								\
	class ObjectName##_Class;																								\
																															\
	template<typename T>																									\
	class ObjectName##_Class<T> : public Class {																			\
	public:																													\
		friend class ObjectName;																							\
																															\
	public:																													\
		template<typename Type>																								\
		inline static constexpr bool Derives() noexcept {																	\
			return std::is_same<Type, T>::value;																			\
		}																													\
																															\
	protected:                                             																	\
		explicit inline ObjectName##_Class(uint32_t ID) noexcept : Class(ID) {}												\
	};																														\
																															\
	template<typename T, typename... Types>																					\
	class ObjectName##_Class<T, Types...> : public ObjectName##_Class<Types...> {											\
		typedef ObjectName##_Class<Types...> Inherited;																		\
																															\
	public:																													\
		friend class ObjectName;																							\
																															\
	public:																													\
		template<typename Type>																								\
		inline static constexpr bool Derives() noexcept {																	\
			return std::is_same<Type, T>::value || Inherited::template Derives<Type>();										\
		}																													\
																															\
	protected:																												\
		explicit inline ObjectName##_Class(uint32_t ID) noexcept : ObjectName##_Class<Types...>(ID) {}						\
	};																														\
																															\
	using ClassType = ObjectName##_Class<Super, ##__VA_ARGS__>;																\
	inline static const ClassType* staticClass = new ClassType(STRING_HASH(#ObjectName));									\



// ====================================================================================

class Interface1 {
	virtual void test() = 0;
};

class Interface2 {};

template<typename T>
class Interface3 {};

class ExampleDerivedObject : public Object, public Interface1, public Interface2 {
	static_assert(!std::is_abstract<Object>());

	GENERATED_BODY(ExampleDerivedObject, Object, Interface1, Interface2)

	virtual void test() override {}
};

class SecondExampleDerivedObject : public Object, public Interface1 {
	GENERATED_BODY(SecondExampleDerivedObject, Object, Interface1)
};

class DoubleDerivedObject : public SecondExampleDerivedObject {
	GENERATED_BODY(DoubleDerivedObject, SecondExampleDerivedObject);
};

class TestTemplateObject : public SecondExampleDerivedObject, public Interface3<int> {
	GENERATED_BODY(TestTemplateObject, SecondExampleDerivedObject, Interface3<int>);
};

// ====================================================================================

#endif //CMF_OBJECT_H