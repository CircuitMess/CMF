#ifndef CMF_STRONGOBJECTPTR_H
#define CMF_STRONGOBJECTPTR_H

#include "ObjectPtr.h"

template<typename T>
class StrongObjectPtr : public ObjectPtr<T, true> {};

#endif //CMF_STRONGOBJECTPTR_H