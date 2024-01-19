#ifndef CMF_SUBCLASSOF_H
#define CMF_SUBCLASSOF_H

#include <concepts>
#include "Object.h"

template<typename T, typename = std::enable_if<std::derived_from<T, Object>, T>::type>
class SubclassOf {
public:
	inline SubclassOf(const Object* object) noexcept {
		if(object == nullptr){
			return;
		}

		if(object->isA<T>()){
			subclass = object->getStaticClass();
		}
	}

	inline SubclassOf(const Class* cls) noexcept {
		if(cls == nullptr){
			return;
		}

		Object* object = cls->createDefaultObject();
		if(object->isA<T>()){
			subclass = cls;
		}

		delete object;
	}

	inline SubclassOf(nullptr_t) noexcept {}

	virtual ~SubclassOf() noexcept = default;

	const Class* operator *() const noexcept {
		return subclass;
	}

	const Class* operator ->() const noexcept {
		return subclass;
	}

	bool operator == (const SubclassOf& other) const noexcept {
		return subclass == other.subclass;
	}

	bool operator == (const Class* other) const noexcept {
		return subclass == other;
	}

	bool operator == (nullptr_t) const noexcept {
		return subclass == nullptr;
	}

private:
	const Class* subclass = nullptr;
};

#endif //CMF_SUBCLASSOF_H