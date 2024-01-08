#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <concepts>
#include <type_traits>
#include "Class.h"
#include "Misc/Djb.h"

class Object {
public:
	Object() noexcept = default;
	virtual ~Object() noexcept = default;

	inline static const Class* staticClass() noexcept {
		return objectStaticClass;
	}

	inline virtual const Class* getStaticClass() const noexcept {
		return staticClass();
	}

	inline virtual bool isA(const Class* other) const noexcept {
		return other->getID() == staticClass()->getID();
	}

	template<typename T>
	inline bool isA() const noexcept {
		return isA(T::staticClass());
	}

	template<typename T>
	static inline constexpr bool implements() noexcept {
		return false;
	}

private:
	using ClassType = Class;
	inline static const ClassType* objectStaticClass = new ClassType(STRING_HASH("Object"));
};

#define GENERATED_BODY(ObjectName, SuperObject, ...) 																		\
	static_assert(std::derived_from<SuperObject, Object>, "Object must have and inherit a base Object class.");				\
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
		inline static constexpr bool implements() noexcept {                 												\
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
		inline static constexpr bool implements() noexcept {                 												\
			return std::is_same<Type, T>::value || Inherited::template implements<Type>();									\
		}																													\
																															\
	protected:																												\
		explicit inline ObjectName##_Class(uint32_t ID) noexcept : ObjectName##_Class<Types...>(ID) {}						\
	};																														\
																															\
	using Super = SuperObject;																								\
	using ClassType = ObjectName##_Class<Super, ##__VA_ARGS__>;																\
	inline static const ClassType* objectStaticClass = new ClassType(STRING_HASH(#ObjectName));								\
																															\
public:																														\
	inline static const Class* staticClass() noexcept {																		\
		return objectStaticClass;																							\
	}																														\
																															\
	inline virtual const Class* getStaticClass() const noexcept override {													\
		return staticClass();																								\
	}																														\
																															\
	inline virtual bool isA(const Class* other) const noexcept override {													\
		return other->getID() == staticClass()->getID() || Super::isA(other);												\
	}																														\
																															\
	template<typename T>																									\
	inline bool isA() const noexcept {																						\
		return isA(T::staticClass());																						\
	}																														\
																															\
	template<typename T>																									\
	inline static constexpr bool implements() noexcept {																	\
		return objectStaticClass->implements<T>() || Super::implements<T>();												\
	}                                                      																	\


// ====================================================================================

class Interface1 {
	virtual void test() = 0;
};

class Interface2 {};

template<typename T>
class Interface3 {};

class ExampleDerivedObject : public Object, public Interface1, public Interface2 {
	static_assert(!std::is_abstract<Object>());

	GENERATED_BODY(ExampleDerivedObject, Object, Interface2, Interface1)

	virtual void test() override {}
};

class SecondExampleDerivedObject : public Object, public Interface1 {
	GENERATED_BODY(SecondExampleDerivedObject, Object, Interface1)
};

class ThirdExampleDerivedObject : public Object, public Interface2 {
	GENERATED_BODY(ThirdExampleDerivedObject, Object, Interface2)
};

class DoubleDerivedObject : public SecondExampleDerivedObject {
	GENERATED_BODY(DoubleDerivedObject, SecondExampleDerivedObject)
};

class TestTemplateObject : public SecondExampleDerivedObject, public Interface3<int> {
	GENERATED_BODY(TestTemplateObject, SecondExampleDerivedObject, Interface3<int>)
};

// ====================================================================================

#endif //CMF_OBJECT_H