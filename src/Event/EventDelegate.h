#ifndef CMF_EVENTDELEGATE_H
#define CMF_EVENTDELEGATE_H

#include "Event.h"

/**
 * @brief 
 * @tparam Args 
 */
template<typename ...Args>
class EventDelegate : public Event<Args...> {
public:
	/**
	 * @brief 
	 * @param args 
	 * @param wait 
	 * @param caller 
	 * @return 
	 */
	inline bool blockingBroadcast(const Args&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept {
		return Event<Args...>::_broadcast(args..., wait);
	}
};

/**
 * @brief 
 * @param EventName 
 */
#define DECLARE_DELEGATE(EventName, ...) class EventName : public EventDelegate<__VA_ARGS__> {};

#endif //CMF_EVENTDELEGATE_H