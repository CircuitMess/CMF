#ifndef CMF_OBJECT_H
#define CMF_OBJECT_H

#include <freertos/FreeRTOS.h>
#include <concepts>
#include <type_traits>
#include <mutex>
#include <freertos/portmacro.h>
#include <atomic>
#include "Misc/Djb.h"
#include "Misc/TemplateTypes.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "Containers/Queue.h"
#include "Containers/Archive.h"
#include "ObjectConstruct.h"
#include "Class.h"

/**
 * @brief The Object class is the backbone of the whole framework, and the basis behind most functionality.
 * The object contains functionality regarding: type-safe casting without rtti, event and event handle ownership,
 * garbage collection, ownership, instigators, serialization and class distinctions of object types.
 */
class Object {
public:
	/**
	 * @brief Default empty constructor, sets the id of the object and locks the destruction mutex of the object.
	 */
	Object() noexcept;

	/**
	 * @brief Default destructor.
	 */
	virtual ~Object() noexcept;

	/**
	 * @return Returns the ID of the object. This ID is unique for every object.
	 */
	inline constexpr uint32_t getID() const noexcept{
		return id;
	}

	/**
	 * @return The name of the object, which is comprised of its class name and the objects ID;
	 */
	std::string getName() const noexcept;

	/**
	 * @return The static class of the object.
	 */
	static inline constexpr const Class* staticClass() noexcept{
		return &objectStaticClass;
	}

	/**
	 * @return The static class of the object instance. This is the same as the static class.
	 */
	inline virtual const Class* getStaticClass() const noexcept{
		return staticClass();
	}

	/**
	 * @brief Checks if this object is of type given in the parameter, or derived from it.
	 * @param other The type class being compared to.
	 * @return True if same type or derived from it, false otherwise.
	 */
	inline constexpr virtual bool isA(const Class* other) const noexcept {
		if(other == nullptr) {
			return false;
		}

		return other->getID() == staticClass()->getID();
	}

	/**
	 * @brief Checks if this object is of type given in the template, or derived from it.
	 * @tparam __T The type being compared to. Has to have a static implementation of 'staticClass()' function.
	 * @return True if same type or derived from it, false otherwise.
	 */
	template<typename __T>
	inline constexpr bool isA() const noexcept{
		return isA(__T::staticClass());
	}

	/**
	 * @brief Checks if this object implements the interface given in the template.
	 * @tparam __T The interface being checked for.
	 * @return True if the object type implements __T interface, false otherwise.
	 */
	template<typename __T>
	static inline constexpr bool implements() noexcept{
		return false;
	}

	/**
	 * @brief Called after the object gets constructed, and is called in the same thread.
	 * Can be used to call virtual functions in the object construction stage.
	 */
	virtual void postInitProperties() noexcept;

	virtual void __postInitProperties() noexcept;

	void operator delete(void* ptr) noexcept;

	/**
	 * @brief Sets the owner of the object.
	 * @param object The new owner.
	 */
	virtual void setOwner(Object* object) noexcept;

	/**
	 * @brief Called after the owner gets changed.
	 * @param oldOwner The old owner.
	 */
	virtual void onOwnerChanged(Object* oldOwner) noexcept;

	/**
	 * @brief Called after a child gets added.
	 * @param child The added child.
	 */
	virtual void onChildAdded(Object* child) noexcept;

	/**
	 * @brief Called after a child gets removed.
	 * @param child The removed child.
	 */
	virtual void onChildRemoved(Object* child) noexcept;

	/**
	 * @brief A function used to iterate over each child until a criteria is met, using a given callback function on each.
	 * @param function The callback function executed for each child until true is returned from it.
	 */
	void forEachChild(const std::function<bool(Object*)>& function) noexcept;

	/**
	 * @return The owner of the object.
	 */
	inline constexpr Object* getOwner() const noexcept{
		if(!owner.isValid()){
			return nullptr;
		}

		return *owner;
	}

	/**
	 * @brief Sets the instigator to the object.
	 * @param object The new instigator.
	 */
	void setInstigator(Object* object) noexcept;

	/**
	 * @brief Called after an instigator gets changed.
	 * @param oldInstigator The old instigator.
	 */
	virtual void onInstigatorChanged(Object* oldInstigator) noexcept;

	/**
	 * @return The instigator of the object.
	 */
	inline constexpr Object* getInstigator() const noexcept{
		return instigator.get();
	}

	/**
	 * @brief Scans all events of the object, triggering their callback execution.
	 * @param wait The maximum wait time for the scanning to get completed.
	 */
	void scanEvents(TickType_t wait) noexcept;

	/**
	 * @brief Registers the event handle owner by this object to the object for future scanning and execution.
	 * @param handle The handle being registered.
	 */
	void registerEventHandle(class EventHandleBase* handle) const noexcept;

	/**
	 * @brief Removes a registered handle from this object.
	 * @param handle The handle being removed.
	 */
	void unregisterEventHandle(EventHandleBase* handle) noexcept;

	/**
	 * @brief Sets an event handle which is ready for callback execution
	 */
	void readyEventHandle(EventHandleBase* handle) noexcept;

	/**
	 * @brief Serializes the object to the archive / deserializes the object from the archive.
	 * @param archive The archive containing the data of the object before serialization / after deserialization.
	 * @return The reference to the archive after serialization / deserialization.
	 */
	virtual Archive& serialize(Archive& archive) noexcept;

	/**
	 * @return The static instance of the Application.
	 */
	static class Application* getApp() noexcept;

	/**
	 * @return The top-most owner in the owner tree.
	 */
	Object* getOutermostOwner() const noexcept;

	/**
	 * @return The top-most instigator in the instigator tree.
	 */
	Object* getOutermostInstigator() const noexcept;

public:
	/**
	 * @return The template names used for information about template types of the instance for the class ID hash.
	 * Public so that GenericClass (declared at namespace scope) can read it via the Derived class.
	 */
	inline static constexpr std::string __getTemplateNames() noexcept{
		return "";
	}

	/**
	 * @return The hash of all template names used for the class ID.
	 * Public for the same reason as __getTemplateNames.
	 */
	inline static constexpr uint32_t __getTemplateHash() noexcept{
		return 0;
	}

	/**
	 * @brief The string identifier of this class, used by GenericClass::getName().
	 * Re-declared by GENERATED_BODY for each derived class.
	 */
	static constexpr const char* __className = "Object";

private:
	using ClassType = Class;
	static constexpr ClassType objectStaticClass{static_cast<uint64_t>(STRING_HASH("Object")) << 32};
	static const ClassRegistrar __objectClassRegistrar;

	inline static uint32_t ObjectIndex = 0;

	const uint32_t id;

	WeakObjectPtr<Object> owner;
	std::set<WeakObjectPtr<Object>> childrenObjects;
	WeakObjectPtr<Object> instigator;

	Queue<EventHandleBase*> readyEventHandles;

	// Thread safety
	std::mutex ownershipMutex;
	std::mutex instigatorMutex;
	std::mutex destroyMutex;
	std::mutex eventScanningMutex;

private:
	/**
	 * @brief Internal child registration function. Used by other objects and not only by this one.
	 * @param child The child being registered.
	 */
	void registerChild(Object* child) noexcept;

	/**
	 * @brief Internal child removal function. Used by other objects and not only by this one.
	 * @param child The child being removed.
	 */
	void removeChild(Object* child) noexcept;
};

/**
 * @brief Macro used by classes that extend objects and are templated themselves to generate functionality needed to cast them safely depending on the type in the template.
 * This macro should be defined at the beginning of the class in the header file.
 * @param T1 The templates of the class (1 or more).
 */
#define TEMPLATE_ATTRIBUTES(T1, ...)																												        \
protected:																																			        \
	inline static constexpr std::string __getTemplateNames() noexcept { return TemplateTypesInfo<T1, ##__VA_ARGS__>::TypeNames(); }							\
	inline static constexpr uint32_t __getTemplateHash() noexcept { return TemplateTypesInfo<T1, ##__VA_ARGS__>::TypesHash(); }						        \
private:																																			        \

#define TEMPLATED_TYPE(...) __VA_ARGS__

#define CONSTRUCTOR_PACK(...) __VA_ARGS__

/**
 * @brief Macro used by classes that extend objects (templated and not) to generate functionality needed for class distinguishing, casting and inheritance functionality.
 * This macro should be defined at the beginning of the class in the header file.
 * @param ObjectName The name of this class.
 * @param SuperObject The object-base class being extended by this class.
 * @param ConstructorTypes Comma-separated constructor argument types wrapped in CONSTRUCTOR_PACK(...), or 'void' for a default constructor.
 * @param ... The interfaces being implemented by this class.
 */
#define GENERATED_BODY(ObjectName, SuperObject, ConstructorTypes, ...) 																		                				\
	static_assert(std::derived_from<SuperObject, Object>, "CMF: Object must have and inherit a base Object class."); 		                                				\
	static_assert(!std::is_abstract<SuperObject>(), "CMF: Extended object cannot be abstract class.");																		\
																																											\
	friend struct ObjectConstruct<ObjectName, ConstructorTypes>;																											\
																															                                				\
public:																														                                				\
	using Super = SuperObject;																								                                				\
	using ClassType = GenericClass<ObjectName, Super, TypeList<ConstructorTypes>, ##__VA_ARGS__>;																			\
	friend ClassType;																																						\
																															                                				\
	static constexpr const char* __className = #ObjectName;																													\
	inline static constexpr ClassType objectStaticClass{((uint64_t) STRING_HASH(#ObjectName) << 32) | __getTemplateHash()};													\
	inline static const ClassRegistrar __##ObjectName##_classRegistrar{&objectStaticClass};																					\
																															                                				\
	static inline constexpr const Class* staticClass() noexcept {															                                				\
		return &objectStaticClass;																							                                				\
	}																														                                				\
																															                                				\
	inline virtual const Class* getStaticClass() const noexcept override {													                                				\
		return staticClass();																								                                				\
	}																														                                				\
																															                                				\
	inline constexpr virtual bool isA(const Class* other) const noexcept override {											                                				\
		return other != nullptr && (other->getID() == staticClass()->getID() || Super::isA(other));							                                				\
	}																														                                				\
																															                                				\
	template<typename __T>																									                                				\
	inline constexpr bool isA() const noexcept {																			                                				\
		return isA(__T::staticClass());																						                                				\
	}																														                                				\
																															                                				\
	template<typename __T>																									                                				\
	inline static constexpr bool implements() noexcept {																	                                				\
		return ClassType::template implements<__T>() || Super::template implements<__T>();									                                				\
	}                                                      																	                                				\
private:																																									\

#endif //CMF_OBJECT_H
