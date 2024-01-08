#ifndef CMF_INTERFACE_H
#define CMF_INTERFACE_H

#include <concepts>
#include "Object.h"
#include "Memory/Cast.h"

template<typename I, typename = std::enable_if<!std::derived_from<I, Object>, I>::type>
class Interface {
public:
	template<typename O, typename = std::enable_if<std::derived_from<O, Object>, O>::type>
	inline constexpr Interface(O* object) noexcept {
		if(object == nullptr){
			return;
		}

		if(!object->template implements<I>()){
			return;
		}

		this->object = cast<Object>(object);
		this->interface = (I*) object;
	}

	inline constexpr Interface(const Interface& other) noexcept {
		if(!other){
			return;
		}

		object = other.getObject();
		interface = other.getInterface();
	}

	inline constexpr Interface(Interface&& other) noexcept {
		if(!other){
			return;
		}

		object = other.getObject();
		interface = other.getInterface();

		other.object = nullptr;
		other.interface = nullptr;
	}

	virtual ~Interface() noexcept = default;

	inline constexpr Object* getObject() const noexcept {
		// TODO: check validity of the object
		return object;
	}

	inline constexpr I* getInterface() const noexcept {
		return interface;
	}

	inline constexpr explicit operator bool() const noexcept {
		return getObject() != nullptr && getInterface() != nullptr; // TODO: check for validity of object pointer instead of nullptr equivalence
	}

	inline constexpr I* operator *() const noexcept {
		return getInterface();
	}

	inline constexpr I* operator ->() const noexcept {
		return getInterface();
	}

private:
	Object* object = nullptr;
	I* interface = nullptr;
};

#endif //CMF_INTERFACE_H