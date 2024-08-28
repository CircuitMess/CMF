#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <freertos/FreeRTOS.h>
#include <concepts>
#include <type_traits>
#include <mutex>
#include <freertos/portmacro.h>
#include <atomic>
#include "Class.h"
#include "Misc/Djb.h"
#include "Misc/TemplateTypes.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Containers/Queue.h"
#include "Containers/Archive.h"

class Object {
public:
	Object() noexcept;
	virtual ~Object() noexcept = default;

	inline constexpr uint32_t getID() const noexcept {
		return id;
	}

	inline std::string getName() const noexcept {
		return getStaticClass()->getName().append("_").append(std::to_string(getID()));
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

	template<typename __T>
	inline bool isA() const noexcept {
		return isA(__T::staticClass());
	}

	template<typename __T>
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

	virtual void setOwner(Object* object) noexcept;
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

	static class Application* getApp() noexcept;

	Object* getOutermostOwner() const noexcept;
	Object* getOutermostInstigator() const noexcept;

protected:
	inline static constexpr std::string __getTemplateNames() noexcept { return ""; }
	inline static constexpr uint32_t __getTemplateHash() noexcept { return 0; }

private:
	using ClassType = Class;
	inline static const ClassType* objectStaticClass = new ClassType((uint64_t) STRING_HASH("Object") << 32);
	inline static uint32_t ObjectIndex = 0;

	const uint32_t id;

	std::atomic<bool> markedForDestroy;
	WeakObjectPtr<Object> owner;
	std::set<WeakObjectPtr<Object>> childrenObjects;
	WeakObjectPtr<Object> instigator;

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

#define TEMPLATE_ATTRIBUTES(T1, ...)																												        \
protected:																																			        \
	inline static constexpr std::string __getTemplateNames() noexcept { return TemplateTypesInfo<T1, ##__VA_ARGS__>::TypeNames(); }							\
	inline static constexpr uint32_t __getTemplateHash() noexcept { return TemplateTypesInfo<T1, ##__VA_ARGS__>::TypesHash(); }						        \
private:																																			        \

#define GENERATED_BODY(ObjectName, SuperObject, ...) 																		                                \
	static_assert(std::derived_from<SuperObject, Object>, "CMF: Object must have and inherit a base Object class."); 		                                \
	static_assert(!std::is_abstract<SuperObject>(), "CMF: Extended object cannot be abstract class.");						                                \
																															                                \
private:																													                                \
	template<typename... __Types>																							                                \
	class __##ObjectName##_Class;																							                                \
																															                                \
	template<typename __T>																									                                \
	class __##ObjectName##_Class<__T> : public Class {																		                                \
	public:																													                                \
		friend class ObjectName;																							                                \
																															                                \
	public:																													                                \
		template<typename __Type>																							                                \
		inline static constexpr bool implements() noexcept {                 												                                \
			return std::is_same<__Type, __T>::value;																		                                \
		}                                                                    												                                \
                                                                       														                                \
        inline virtual Object* createDefaultObject() const noexcept override {  											                                \
			void* temp = operator new(sizeof(ObjectName));																	                                \
			memset(temp, 0, sizeof(ObjectName));																			                                \
			return new(temp) ObjectName();																					                                \
		}                                                                    												                                \
																															                                \
		inline virtual constexpr std::string getName() const noexcept override{												                                \
			const std::string templates = __getTemplateNames();																								\
			const std::string ret = std::string(#ObjectName) + (templates.empty() ? "" : "<" + templates + ">");											\
			return ret;																																		\
		}																													                                \
																															                                \
	protected:                                             																	                                \
		explicit inline __##ObjectName##_Class(uint64_t ID) noexcept : Class(ID) {}											                                \
	};																														                                \
																															                                \
	template<typename __T, typename... __Types>																				                                \
	class __##ObjectName##_Class<__T, __Types...> : public __##ObjectName##_Class<__Types...> {								                                \
		typedef __##ObjectName##_Class<__Types...> Inherited;																                                \
																															                                \
	public:																													                                \
		friend class ObjectName;																							                                \
																															                                \
	public:																													                                \
		template<typename __Type>																							                                \
		inline static constexpr bool implements() noexcept {                 												                                \
			return std::is_same<__Type, __T>::value || Inherited::template implements<__Type>();							                                \
		}																													                                \
																															                                \
	protected:																												                                \
		explicit inline __##ObjectName##_Class(uint64_t ID) noexcept : __##ObjectName##_Class<__Types...>(ID) {}			                                \
	};																														                                \
																															                                \
	using Super = SuperObject;																								                                \
	using ClassType = __##ObjectName##_Class<Super, ##__VA_ARGS__>;															                                \
	inline static const ClassType* objectStaticClass = new ClassType(((uint64_t) STRING_HASH(#ObjectName) << 32) | __getTemplateHash());					\
																															                                \
public:																														                                \
	inline static const Class* staticClass() noexcept {																		                                \
		return objectStaticClass;																							                                \
	}																														                                \
																															                                \
	inline virtual const Class* getStaticClass() const noexcept override {													                                \
		return staticClass();																								                                \
	}																														                                \
																															                                \
	inline virtual bool isA(const Class* other) const noexcept override {													                                \
		return other->getID() == staticClass()->getID() || Super::isA(other);												                                \
	}																														                                \
																															                                \
	template<typename __T>																									                                \
	inline bool isA() const noexcept {																						                                \
		return isA(__T::staticClass());																						                                \
	}																														                                \
																															                                \
	template<typename __T>																									                                \
	inline static constexpr bool implements() noexcept {																	                                \
		return objectStaticClass->template implements<__T>() || Super::template implements<__T>();							                                \
	}                                                      																	                                \


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

template<typename T, typename U>
class TestTemplateObject : public SecondExampleDerivedObject, public Interface3<int> {
	TEMPLATE_ATTRIBUTES(T, U)
	GENERATED_BODY(TestTemplateObject, SecondExampleDerivedObject, Interface3)
};

// ====================================================================================

#endif //CMF_OBJECT_H