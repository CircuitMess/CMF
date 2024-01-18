#ifndef CMF_EVENT_H
#define CMF_EVENT_H

#include <set>
#include "EventHandle.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

// TODO: turn it into thread-safe event class
// TODO: in derived classes, if there has to be a limit to only owner being able to call broadcast, this should be done using macros
// TODO: the ownership effect will ensure that the event can be defined in a way that only owner, or only owning class can call it in case owner is null
template<typename ...Args>
class Event {
public:
	inline virtual ~Event() noexcept {
		for(const HandleContainer& container : handles){
			if(container.owned){
				delete container.handle;
			}
		}
	}

	inline void bind(EventHandle<Args...>&& handle) noexcept {
		handles.insert({&handle, false});
	}

	inline void bind(EventHandle<Args...>* handle) noexcept {
		if(handle == nullptr){
			return;
		}

		handles.insert({handle, false});
	}

	template<typename O, typename F>
	inline void bind(O* object, F&& function) noexcept {
		if(object == nullptr || function == nullptr){
			return;
		}

		HandleContainer container;
		container.handle = new EventHandle<Args...>();
		container.handle->bind(object, function);

		handles.insert(container);
	}

	inline void bind(Object* object, const std::function<void(Args...)>& function) noexcept {
		if(object == nullptr || function == nullptr){
			return;
		}

		HandleContainer container;
		container.handle = new EventHandle<Args...>();
		container.handle->bind(object, function);

		handles.insert(container);
	}

protected:
	inline bool broadcast(const Args&... args, TickType_t wait = portMAX_DELAY) noexcept {
		bool succeeded = true;

		for(const HandleContainer& container : handles){
			if(container.handle == nullptr){
				continue;
			}

			succeeded &= container.handle->call(wait, args...);
		}

		return succeeded;
	}

	inline bool broadcast(Args&&... args, TickType_t wait = portMAX_DELAY) noexcept {
		bool succeeded = true;

		for(const HandleContainer& container : handles){
			if(container.handle == nullptr){
				continue;
			}

			succeeded &= container.handle->call(wait, args...);
		}

		return succeeded;
	}

private:
	struct HandleContainer {
		EventHandle<Args...>* handle = nullptr;
		bool owned = true;

		// This is only needed for std::set to work
		bool operator < (const HandleContainer& other) const noexcept {
			return (uint32_t) handle < (uint32_t) other.handle;
		}
	};

	std::set<HandleContainer> handles;
};

#endif //CMF_EVENT_H