#ifndef CMF_EVENTHANDLE_H
#define CMF_EVENTHANDLE_H

// TODO: make sure objects have a way to retrieve max event delay from them, then when scanning for events in a thread
// TODO: event must be waited for at most the amount specified by its owning object. If done in a entity, it cannot wait for events so long as to over-delay next tick
// TODO: wait for each event handle at rate of smallest delay / (num of handles scanned - 1), this ensures that each event will be at most delayed for the minimum delay between them all

#include <concepts>
#include <functional>
#include "Object/Object.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"

// TODO: must create template specialization with size_t typename for each of possible numbers of arguments to be able to bind functions with arguments in a seamless way

template<typename ...Args>
class EventHandle {
protected:
	void bind(std::function<void(Args...)>&& function, Object* object) noexcept {
		switch(sizeof...(Args)){
			case 1: {
				break;
			}
			case 2: {
				break;
			}
			case 3: {
				break;
			}
			case 4: {
				break;
			}
			case 5: {
				break;
			}
			case 6: {
				break;
			}
			case 7: {
				break;
			}
			case 8: {
				break;
			}
			case 9: {
				break;
			}
			case 10: {
				break;
			}
			case 11: {
				break;
			}
			case 12: {
				break;
			}
			case 13: {
				break;
			}
			case 14: {
				break;
			}
			case 15: {
				break;
			}
			case 16: {
				break;
			}
			case 17: {
				break;
			}
			case 18: {
				break;
			}
			case 19: {
				break;
			}
			case 20: {
				break;
			}
			case 21: {
				break;
			}
			case 22: {
				break;
			}
			case 23: {
				break;
			}
			case 24: {
				break;
			}
			case 25: {
				break;
			}
			case 26: {
				break;
			}
			case 27: {
				break;
			}
			case 28: {
				break;
			}
			case 29: {
				break;
			}
			default: {
				// TODO: error
				break;
			}

		}
	}

	void test() {
		std::function<void(void)> t = bind(&EventHandle::test, this);
		sizeof...(Args);
	}
private:
	WeakObjectPtr<Object> owningObject; // Object of which the callback is a member, in case this owning object
	std::function<void(Args...)> callback;
};

#endif //CMF_EVENTHANDLE_H