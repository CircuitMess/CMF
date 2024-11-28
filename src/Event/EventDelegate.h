#ifndef CMF_EVENTDELEGATE_H
#define CMF_EVENTDELEGATE_H

#include "EventBroadcaster.h"

/**
 * @brief Event implementation which anyone can broadcast without limitations.
 * @tparam Args The types of arguments being broadcast to the callback functions.
 */
template<typename ...Args>
class EventDelegate : public Event<Args...> {
public:
	/**
	 * @brief Broadcast function that is public and accessible to anyone.
	 * @param args The arguments being broadcast.
	 * @param wait The wait time per function to trigger the callback.
	 * @param caller The optional caller of the function.
	 * @return True if successful, false otherwise.
	 */
	inline bool blockingBroadcast(const Args&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept {
		return Event<Args...>::_broadcast(args..., wait);
	}
};

/**
 * @brief Macro used to declare the custom type event delegate.
 * @param EventName The custom name of the delegate type.
 */
#define DECLARE_DELEGATE(EventName, ...) class EventName : public EventDelegate<__VA_ARGS__> {};

#endif //CMF_EVENTDELEGATE_H