#ifndef CMF_CLASS_H
#define CMF_CLASS_H

#include <cinttypes>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include "Memory/SmartPtr/StrongObjectPtr.h"
#include "ObjectConstruct.h"

class Object;
class Class;

/**
 * @brief Class registry is used internally by CMF to track all existing object classes by their ID.
 */
class ClassRegistry {
public:
	/**
	 * @brief Default destructor.
	 */
	virtual ~ClassRegistry() noexcept = default;

	/**
	 * @param ID ID of the wanted class.
	 * @return The class matching the given ID. If no class matches it, returns nullptr.
	 */
	const Class* getClass(uint64_t ID) const noexcept;

	/**
	 * @brief Registers a class. Each class is registered only once at static initialization time
	 * via a ClassRegistrar helper instance.
	 * @param cls
	 */
	void registerClass(const Class* cls) noexcept;

private:
	std::map<uint64_t, const Class*> classes;
};

/**
 * @brief Class distinguishes the Objects.
 * Each object has a unique class which is implemented uniquely depending on the object.
 * Classes are mainly used for Object casting and construction.
 */
class Class {
	friend class Object;
	friend class ClassRegistrar;

public:
	/**
	 * @brief Constructor of the class with given ID. The constructor is constexpr so that
	 * Class instances can be used as static constexpr values, allowing constexpr SubclassOf usage.
	 * The ID is generated partially at compile time, and partially at the beginning of the runtime statically.
	 * @param ID The generated ID of the class.
	 */
	constexpr explicit Class(uint64_t ID) noexcept : classID(ID) {}

	/**
	 * @brief Default constexpr destructor.
	 */
	constexpr virtual ~Class() noexcept = default;

	/**
	 * @return An object represented by this class.
	 */
	template<typename ...Args>
	StrongObjectPtr<Object> createObject(Args&&... args) const noexcept {
		std::tuple<Args...> arguments = std::make_tuple(std::forward<Args>(args)...);
		return __createObject(&arguments);
	}

	/**
	 * @return The ID of the class. Each class has a unique ID
	 * depending on the name of the object it represents and the templates types of that object.
	 */
	constexpr uint64_t getID() const noexcept {
		return classID;
	}

	/**
	 * @tparam Type The type of interface that the object represented by this class implements.
	 * @return True if the represented object implements the template interface.
	 */
	template<typename Type>
	static inline constexpr bool implements() noexcept {
		return false;
	}

	/**
	 * @brief Checks if this class is of type given in the parameter, or derived from it.
	 * @param other The type class being compared to.
	 * @return True if same type or derived from it, false otherwise.
	 */
	constexpr virtual bool isA(const Class* other) const noexcept {
		if(other == nullptr) {
			return false;
		}

		return other->getID() == getID();
	}

	/**
	 * @brief Checks if this class is of type given in the template, or derived from it.
	 * @tparam __T The type of Object being compared to. Has to have a static implementation of 'staticClass()' function.
	 * @return True if same type or derived from it, false otherwise.
	 */
	template<typename __T>
	inline constexpr bool isA() const noexcept {
		return isA(__T::staticClass());
	}

	/**
	 * @param ID ID of the given class.
	 * @return The class matching the ID from the ClassRegistry.
	 */
	static inline const Class* getClasByID(uint64_t ID) noexcept {
		if(registry == nullptr) {
			return nullptr;
		}

		return registry->getClass(ID);
	}

	/**
	 * @return The name of the object type the class represents.
	 */
	inline virtual constexpr std::string getName() const noexcept {
		return "Object";
	}

protected:
	static inline ClassRegistry* registry = nullptr;

protected:
	/**
	 * @return An object represented by this class.
	 */
	virtual StrongObjectPtr<Object> __createObject(void* arguments) const noexcept;

private:
	uint64_t classID;
};

/**
 * @brief Helper used at static initialization to register a class instance with the ClassRegistry.
 * Registration is moved out of the Class constructor so that Class can have a constexpr constructor.
 */
class ClassRegistrar {
public:
	explicit ClassRegistrar(const Class* cls) noexcept {
		if(Class::registry == nullptr) {
			Class::registry = new ClassRegistry();
		}

		Class::registry->registerClass(cls);
	}
};

/**
 * @brief Type list used to wrap a parameter pack of types to be passed as a single template argument.
 */
template<typename... Types>
struct TypeList {};

/**
 * @brief Generic templated Class implementation used by GENERATED_BODY for any object type.
 * Defined at namespace scope (not nested inside the user class) so that its constexpr constructor
 * is fully parsed by the time the user class's static constexpr instance is initialized.
 *
 * @tparam Derived The user-defined Object subclass that this Class describes.
 * @tparam Super The direct base Object subclass of Derived.
 * @tparam CtorList TypeList of constructor argument types for Derived (or TypeList<void> for default constructor).
 * @tparam Interfaces Pack of interface types implemented by Derived.
 */
template<typename Derived, typename Super, typename CtorList, typename... Interfaces>
class GenericClass;

template<typename Derived, typename Super, typename... CtorArgs, typename... Interfaces>
class GenericClass<Derived, Super, TypeList<CtorArgs...>, Interfaces...> : public Class {
public:
	constexpr explicit GenericClass(uint64_t ID) noexcept : Class(ID) {}

	constexpr virtual ~GenericClass() noexcept override = default;

	template<typename T>
	static inline constexpr bool implements() noexcept {
		if constexpr (sizeof...(Interfaces) > 0) {
			return std::is_same_v<T, Super> || (std::is_same_v<T, Interfaces> || ...);
		} else {
			return std::is_same_v<T, Super>;
		}
	}

	constexpr virtual bool isA(const Class* other) const noexcept override {
		if(other == nullptr) {
			return false;
		}

		return other->getID() == Class::getID() || Super::staticClass()->isA(other);
	}

	template<typename T>
	inline constexpr bool isA() const noexcept {
		return isA(T::staticClass());
	}

	inline virtual constexpr std::string getName() const noexcept override {
		const std::string templates = Derived::__getTemplateNames();
		return std::string(Derived::__className) + (templates.empty() ? "" : "<" + templates + ">");
	}

	inline virtual StrongObjectPtr<Object> __createObject(void* arguments) const noexcept override {
		void* temp = operator new(sizeof(Derived));
		if(temp == nullptr) {
			return nullptr;
		}

		memset(temp, 0, sizeof(Derived));

		StrongObjectPtr<Derived> tempPtr = static_cast<Derived*>(temp);
		ObjectConstruct<Derived, CtorArgs...> construct(arguments);

		return construct.create(temp);
	}
};

// Bring in Object so that headers which historically included only Class.h still see Object.
// Object.h includes Class.h, but its include guard prevents recursion: when Object.h is the
// translation entry, this trailing include is a no-op; when Class.h is the entry, Object.h is
// pulled in here after Class and GenericClass are fully defined.
#include "Object.h"

#endif //CMF_CLASS_H
