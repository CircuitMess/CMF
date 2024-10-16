#ifndef CMF_EVENTBROADCASTER_H
#define CMF_EVENTBROADCASTER_H

#include "Event.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

/**
 * @brief 
 * @tparam Args 
 */
template<typename ...Args>
class EventBroadcaster : protected Event<Args...> {
public:
	/**
	 * @brief 
	 */
	inline EventBroadcaster() noexcept = delete;

	/**
	 * @brief 
	 */
	inline EventBroadcaster(const EventBroadcaster&) noexcept = delete;

	/**
	 * @brief 
	 */
	inline EventBroadcaster(EventBroadcaster&&) noexcept = delete;

	/**
	 * @brief 
	 * @param owningObject 
	 */
	inline explicit EventBroadcaster(Object* owningObject) noexcept : owningObject(owningObject) {}

	/**
	 * @brief 
	 * @param handle 
	 */
	inline void bind(EventHandle<Args...>&& handle) noexcept {
		Event<Args...>::bind(handle);
	}

	/**
	 * @brief 
	 * @param handle 
	 */
	inline void bind(EventHandle<Args...>* handle) noexcept {
		Event<Args...>::bind(handle);
	}

	/**
	 * @brief 
	 * @tparam O 
	 * @tparam F 
	 * @param object 
	 * @param function 
	 */
	template<typename O, typename F>
	inline void bind(O* object, F&& function) noexcept {
		Event<Args...>::bind(object, function);
	}

	/**
	 * @brief 
	 * @param object 
	 * @param function 
	 */
	inline void bind(Object* object, const std::function<void(Args...)>& function) noexcept {
		Event<Args...>::bind(object, function);
	}

protected:
	/**
	 * @brief 
	 * @param args 
	 * @param wait 
	 * @param caller 
	 * @return 
	 */
	inline bool blockingBroadcast(const Args&... args, TickType_t wait = portMAX_DELAY, Object* caller = nullptr) noexcept{
		if(caller != owningObject){
			return false;
		}

		return Event<Args...>::_broadcast(args..., wait);
	}

private:
	WeakObjectPtr<Object> owningObject;
};

/**
 * @brief 
 */
#define broadcast(...) blockingBroadcast(__VA_ARGS__ __VA_OPT__(,) portMAX_DELAY, this)

/**
 * @brief 
 * @param wait 
 */
#define waitBroadcast(wait, ...) blockingBroadcast(__VA_ARGS__ __VA_OPT__(,) wait, this)

/**
 * @brief 
 * @param EventName 
 * @param OwningType 
 */
#define DECLARE_EVENT(EventName, OwningType, ...)																\
class EventName : public EventBroadcaster<__VA_ARGS__> {														\
	friend class OwningType;																					\
	inline explicit EventName(Object* owner) noexcept : EventBroadcaster<__VA_ARGS__>(owner) {}					\
};																												\

#endif //CMF_EVENTBROADCASTER_H