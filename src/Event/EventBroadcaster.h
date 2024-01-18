#ifndef CMF_EVENTBROADCASTER_H
#define CMF_EVENTBROADCASTER_H

#include "Event.h"

// TODO ownership
template<typename ...Args>
class EventBroadcaster : protected Event<Args...> {
public:
	inline void bind(EventHandle<Args...>&& handle) noexcept {
		Event<Args...>::bind(handle);
	}

	inline void bind(EventHandle<Args...>* handle) noexcept {
		Event<Args...>::bind(handle);
	}

	template<typename O, typename F>
	inline void bind(O* object, F&& function) noexcept {
		Event<Args...>::bind(object, function);
	}

	inline void bind(Object* object, const std::function<void(Args...)>& function) noexcept {
		Event<Args...>::bind(object, function);
	}

protected:
	inline bool blockingBroadcast(const Args&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept{
		return Event<Args...>::broadcast(args..., wait);
	}

	inline bool blockingBroadcast(Args&&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept{
		return Event<Args...>::broadcast(args..., wait);
	}
};

#define broadcastObject(...) blockingBroadcast(__VA_ARGS__, portMAX_DELAY, this)

#define waitBroadcast(wait, ...) blockingBroadcast(__VA_ARGS__, wait, this)

#define DECLARE_EVENT(EventName, OwningType, ...)							\
class EventName : public EventBroadcaster<__VA_ARGS__> {					\
	friend class OwningType;												\
};																			\

#endif //CMF_EVENTBROADCASTER_H