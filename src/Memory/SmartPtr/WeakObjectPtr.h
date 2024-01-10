#ifndef CMF_WEAKOBJECTPTR_H
#define CMF_WEAKOBJECTPTR_H

#include "ObjectPtr.h"

template<typename T>
class WeakObjectPtr : public ObjectPtr<T, false> {};

#endif //CMF_WEAKOBJECTPTR_H
