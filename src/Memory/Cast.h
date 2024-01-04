#ifndef CMF_CAST_H
#define CMF_CAST_H

#include "Object/Object.h"

template<typename T1, typename T2, typename = std::enable_if<std::derived_from<T1, Object>, T1>::type, typename = std::enable_if<std::derived_from<T2, Object>, T2>::type>
inline static T1* Cast(T2* object){
	if(object == nullptr){
		return nullptr;
	}

	if(object->template IsA<T1>()){
		return (T1*) object;
	}

	return nullptr;
}

#endif //CMF_CAST_H