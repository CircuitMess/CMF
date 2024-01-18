#ifndef CMF_EVENTDELEGATE_H
#define CMF_EVENTDELEGATE_H

#include "Event.h"

// TODO: anyone can call a delegate, but only the owning class can call a broadcaster
template<typename ...Args>
class EventDelegate : public Event<Args...> {
	
};

#endif //CMF_EVENTDELEGATE_H