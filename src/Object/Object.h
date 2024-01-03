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

	// TODO
	/*static_assert(std::derived_from<ObjectName, SuperObject>, "Object must derive from given base object class.");		\
    static_assert(std::derived_from<SuperObject, Object>(), "Base object must be of type object.");							\
    static_assert(sizeof(ObjectName) != 0, "Object is incomplete.");														\
    static_assert(sizeof(SuperObject) != 0, "Base object is incomplete.");													\*/

#define GENERATED_BODY(ObjectName, SuperObject, ...) 																		\
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
	class ObjectName_Class;																									\
																															\
	template<typename T>																									\
	class ObjectName_Class<T> : public Class {																				\
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
		explicit inline ObjectName_Class(uint32_t ID) noexcept : Class(ID) {}												\
	};																														\
																															\
	template<typename T, typename... Types>																					\
	class ObjectName_Class<T, Types...> : public ObjectName_Class<Types...> {												\
		typedef ObjectName_Class<Types...> Inherited;																		\
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
		explicit inline ObjectName_Class(uint32_t ID) noexcept : ObjectName_Class<Types...>(ID) {}							\
	};																														\
																															\
	using ClassType = ObjectName_Class<Super, ##__VA_ARGS__>;																\
	inline static const ClassType* staticClass = new ClassType(STRING_HASH(#ObjectName));									\



// ====================================================================================

class Interface1 {
	virtual void test() = 0;
};

class Interface2 {};

class ExampleDerivedObject : public Object, public Interface1, public Interface2 {
	GENERATED_BODY(ExampleDerivedObject, Object, Interface1, Interface2)

	virtual void test() override {}
};

class SecondExampleDerivedObject : public Object, public Interface1 {
	GENERATED_BODY(SecondExampleDerivedObject, Object, Interface1)
};

class DoubleDerivedObject : public SecondExampleDerivedObject {
	GENERATED_BODY(DoubleDerivedObject, SecondExampleDerivedObject);
};

// ====================================================================================

#endif //CMF_OBJECT_H