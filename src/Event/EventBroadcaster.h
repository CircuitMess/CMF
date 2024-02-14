#ifndef CMF_EVENTBROADCASTER_H
#define CMF_EVENTBROADCASTER_H

#include "Event.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

template<typename ...Args>
class EventBroadcaster : protected Event<Args...> {
public:
	inline EventBroadcaster() noexcept = delete;
	inline EventBroadcaster(const EventBroadcaster&) noexcept = delete;
	inline EventBroadcaster(EventBroadcaster&&) noexcept = delete;
	inline explicit EventBroadcaster(Object* owningObject) noexcept : owningObject(owningObject) {}

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
		if(caller != owningObject){
			return false;
		}

		return Event<Args...>::_broadcast(args..., wait);
	}

private:
	WeakObjectPtr<Object> owningObject;
};

#define broadcast(...) blockingBroadcast(__VA_ARGS__ __VA_OPT__(,) portMAX_DELAY, this)

#define waitBroadcast(wait, ...) blockingBroadcast(__VA_ARGS__ __VA_OPT__(,) wait, this)

#define DECLARE_EVENT(EventName, OwningType, ...)																\
class EventName : public EventBroadcaster<__VA_ARGS__> {														\
	friend class OwningType;																					\
	inline explicit EventName(Object* owner) noexcept : EventBroadcaster<__VA_ARGS__>(owner) {}					\
};																												\

#endif //CMF_EVENTBROADCASTER_H