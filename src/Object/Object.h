#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <concepts>
#include <type_traits>
#include <mutex>
#include "Class.h"
#include "Misc/Djb.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

// TODO: owner and event handle thread safety

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

	virtual void postInitProperties() noexcept;

	virtual void onCreated() noexcept;

	void destroy() noexcept;
	virtual void onDestroy() noexcept;

	inline constexpr bool isMarkedForDestroy() const noexcept{
		return markedForDestroy;
	}

	void setOwner(Object* object) noexcept;
	virtual void onOwnerChanged(Object* oldOwner) noexcept;
	virtual void onChildAdded(Object* child) noexcept;
	virtual void onChildRemoved(Object* child) noexcept;
	void forEachChild(const std::function<bool(Object*)>& function) noexcept;

	inline constexpr Object* getOwner() const noexcept {
		if(!owner.isValid()){
			return nullptr;
		}

		return *owner;
	}

	void setInstigator(Object* object) noexcept;
	virtual void onInstigatorChanged(Object* oldInstigator) noexcept;

	inline constexpr Object* getInstigator() const noexcept {
		return instigator.get();
	}

	virtual void scanEvents() noexcept;
	void registerEventHandle(class EventHandleBase* handle) noexcept;
	void unregisterEventHandle(EventHandleBase* handle) noexcept;

private:
	using ClassType = Class;
	inline static const ClassType* objectStaticClass = new ClassType(STRING_HASH("Object"));

	bool markedForDestroy = false;
	WeakObjectPtr<Object> owner = nullptr;
	std::set<WeakObjectPtr<Object>> childrenObjects;
	WeakObjectPtr<Object> instigator = nullptr;
	std::set<EventHandleBase*> ownedEventHandles; // TODO: think about using smart pointers to manage these

	std::mutex ownershipMutex;
	std::mutex instigatorMutex;
	std::mutex eventHandleMutex;
	std::mutex destroyMutex;

private:
	void registerChild(Object* child) noexcept;
	void removeChild(Object* child) noexcept;
};

#define GENERATED_BODY(ObjectName, SuperObject, ...) 																		\
	static_assert(std::derived_from<SuperObject, Object>, "CMF: Object must have and inherit a base Object class."); 		\
	static_assert(!std::is_abstract<SuperObject>(), "CMF: Extended object cannot be abstract class.");						\
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
		}                                                                    												\
                                                                       														\
        inline virtual Object* createDefaultObject() const noexcept override {  											\
			return new ObjectName();																						\
		}                                                               													\
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
};

class Interface2 {};

template<typename T>
class Interface3 {};

class ExampleDerivedObject : public Object, public Interface1, public Interface2 {
	GENERATED_BODY(ExampleDerivedObject, Object, Interface2, Interface1)
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