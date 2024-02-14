#ifndef CMF_EVENT_H
#define CMF_EVENT_H

#include <set>
#include <mutex>
#include "EventHandle.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

template<typename ...Args>
class Event {
public:
	inline virtual ~Event() noexcept {
		std::lock_guard guard(accessMutex);

		for(const HandleContainer& container : handles){
			if(container.owned){
				delete container.handle;
			}
		}
	}

	inline void bind(EventHandle<Args...>&& handle) noexcept {
		if(handle.getOwningObject() == nullptr){
			return;
		}

		std::lock_guard guard(accessMutex);
		handles.insert({&handle, handle.getOwningObject(), false});
	}

	inline void bind(EventHandle<Args...>* handle) noexcept {
		if(handle == nullptr){
			return;
		}

		if(handle->getOwningObject() == nullptr){
			return;
		}

		std::lock_guard guard(accessMutex);

		handles.insert({handle, handle->getOwningObject(), false});
	}

	template<typename O, typename F>
	inline void bind(O* object, F&& function) noexcept {
		if(object == nullptr || function == nullptr){
			return;
		}

		std::lock_guard guard(accessMutex);

		HandleContainer container;
		container.handle = new EventHandle<Args...>();
		container.handle->bind(object, function);
		container.owningObject = object;

		handles.insert(container);
	}

	inline void bind(Object* object, const std::function<void(Args...)>& function) noexcept {
		if(object == nullptr || function == nullptr){
			return;
		}

		std::lock_guard guard(accessMutex);

		HandleContainer container;
		container.handle = new EventHandle<Args...>();
		container.handle->bind(object, function);
		container.owningObject = object;

		handles.insert(container);
	}

	inline void unbind(Object* object) noexcept {
		std::erase_if(handles, [object](const HandleContainer& container) {
			if(container.owningObject == object){
				if(container.owned){
					delete container.handle;
				}

				return true;
			}
		});
	}

protected:
	inline bool _broadcast(const Args&... args, TickType_t wait = portMAX_DELAY) noexcept {
		std::lock_guard guard(accessMutex);

		bool succeeded = true;

		for(HandleContainer container : handles){
			if(container.handle == nullptr || !container.owningObject.isValid()){
				continue;
			}

			succeeded &= container.handle->call(wait, args...);
		}

		return succeeded;
	}

private:
	struct HandleContainer {
		EventHandle<Args...>* handle = nullptr;
		WeakObjectPtr<Object> owningObject = nullptr;
		bool owned = true;

		// This is only needed for std::set to work
		bool operator < (const HandleContainer& other) const noexcept {
			return (uint32_t) handle < (uint32_t) other.handle;
		}
	};

	std::set<HandleContainer> handles;
	std::mutex accessMutex;
};

#endif //CMF_EVENT_H