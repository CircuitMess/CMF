#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <concepts>
#include <type_traits>
#include <mutex>
#include <freertos/portmacro.h>
#include <atomic>
#include "Class.h"
#include "Misc/Djb.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Containers/Queue.h"
#include "Containers/Archive.h"

// TODO: technically it could happen that static class instance does not initialize before some other object that uses static functions to retrieve it, therefore change it so that it gets initialized the first time it is retrieved

class Object {
public:
	Object() noexcept;
	virtual ~Object() noexcept = default;

	inline constexpr uint32_t getID() const noexcept {
		return id;
	}

	inline std::string getName() const noexcept {
		return std::string(getStaticClass()->getName()).append("_").append(std::to_string(getID()));
	}

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

	bool canDelete() noexcept;

	void destroy() noexcept;
	virtual void onDestroy() noexcept;

	inline bool isMarkedForDestroy() const noexcept{
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

	void scanEvents(TickType_t wait) noexcept;
	void registerEventHandle(class EventHandleBase* handle) noexcept;
	void unregisterEventHandle(EventHandleBase* handle) noexcept;
	virtual TickType_t getEventScanningTime() const noexcept;

	virtual Archive& serialize(Archive& archive) noexcept;

private:
	using ClassType = Class;
	inline static const ClassType* objectStaticClass = new ClassType(STRING_HASH("Object"));
	inline static uint32_t ObjectIndex = 0;

	const uint32_t id;

	std::atomic<bool> markedForDestroy = false;
	WeakObjectPtr<Object> owner = nullptr;
	std::set<WeakObjectPtr<Object>> childrenObjects;
	WeakObjectPtr<Object> instigator = nullptr;

	struct EventHandleContainer {
		EventHandleBase* ownedEventHandle = nullptr;
		StrongObjectPtr<class Threaded> eventThread = nullptr;

		inline constexpr bool operator < (const EventHandleContainer& other) const noexcept {
			return ownedEventHandle < other.ownedEventHandle;
		}
	};

	std::set<EventHandleContainer> ownedEventHandles;
	Queue<EventHandleBase*> readyEventHandles;

	// Thread safety
	std::mutex ownershipMutex;
	std::mutex instigatorMutex;
	std::mutex eventHandleMutex;
	std::mutex destroyMutex;
	std::mutex eventScanningMutex;

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
		}                                                                    												\
																															\
		inline virtual constexpr const char* getName() const noexcept override{												\
			return #ObjectName;																								\
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