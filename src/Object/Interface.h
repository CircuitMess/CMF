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

		if(!object->template Implements<I>()){
			return;
		}

		m_Object = Cast<Object>(object);
		m_Interface = (I*) object;
	}

	inline constexpr Interface(const Interface& other) noexcept {
		if(!other){
			return;
		}

		m_Object = other.GetObject();
		m_Interface = other.GetInterface();
	}

	inline constexpr Interface(Interface&& other) noexcept {
		if(!other){
			return;
		}

		m_Object = other.GetObject();
		m_Interface = other.GetInterface();

		other.m_Object = nullptr;
		other.m_Interface = nullptr;
	}

	virtual ~Interface() noexcept = default;

	inline constexpr Object* GetObject() const noexcept {
		// TODO: check validity of the object
		return m_Object;
	}

	inline constexpr I* GetInterface() const noexcept {
		return m_Interface;
	}

	inline constexpr explicit operator bool() const noexcept {
		return GetObject() != nullptr && GetInterface() != nullptr; // TODO: check for validity of object pointer instead of nullptr equivalence
	}

	inline constexpr I* operator *() const noexcept {
		return GetInterface();
	}

	inline constexpr I* operator ->() const noexcept {
		return GetInterface();
	}

private:
	Object* m_Object = nullptr;
	I* m_Interface = nullptr;
};

#endif //CMF_INTERFACE_H