#ifndef CMF_EVENTDELEGATE_H
#define CMF_EVENTDELEGATE_H

#include "Event.h"

template<typename ...Args>
class EventDelegate : public Event<Args...> {
public:
	inline bool broadcast(const Args&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept {
		return Event<Args...>::broadcast(args..., wait);
	}

	inline bool broadcast(Args&&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept {
		return Event<Args...>::broadcast(args..., wait);
	}
};

#define DECLARE_DELEGATE(EventName, ...) class EventName : public EventDelegate<__VA_ARGS__> {};

#endif //CMF_EVENTDELEGATE_H