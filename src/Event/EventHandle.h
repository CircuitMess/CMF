#ifndef CMF_EVENTHANDLE_H
#define CMF_EVENTHANDLE_H

// TODO: make sure objects have a way to retrieve max event delay from them, then when scanning for events in a thread
// TODO: event must be waited for at most the amount specified by its owning object. If done in a entity, it cannot wait for events so long as to over-delay next tick
// TODO: wait for each event handle at rate of smallest delay / (num of handles scanned - 1), this ensures that each event will be at most delayed for the minimum delay between them all

#include <concepts>
#include <functional>
#include <tuple>
#include <forward_list>
#include "Object/Object.h"
#include "Memory/SmartPtr/WeakObjectPtr.h"
#include "Containers/Queue.h"

template<typename ...Args>
class EventHandle {
protected:
	void bind(std::function<void(Args...)>&& function, Object* object) noexcept {
		owningObject = object;

		switch(sizeof...(Args)){
			case 0: {
				callback = std::bind(function, object);
				break;
			}
			case 1: {
				callback = std::bind(function, object, std::placeholders::_1);
				break;
			}
			case 2: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2);
				break;
			}
			case 3: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3);
				break;
			}
			case 4: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4);
				break;
			}
			case 5: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5);
				break;
			}
			case 6: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6);
				break;
			}
			case 7: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7);
				break;
			}
			case 8: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8);
				break;
			}
			case 9: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9);
				break;
			}
			case 10: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10);
				break;
			}
			case 11: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11);
				break;
			}
			case 12: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12);
				break;
			}
			case 13: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13);
				break;
			}
			case 14: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14);
				break;
			}
			case 15: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15);
				break;
			}
			case 16: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16);
				break;
			}
			case 17: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17);
				break;
			}
			case 18: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18);
				break;
			}
			case 19: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19);
				break;
			}
			case 20: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20);
				break;
			}
			case 21: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21);
				break;
			}
			case 22: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22);
				break;
			}
			case 23: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23);
				break;
			}
			case 24: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24);
				break;
			}
			case 25: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24,
									 std::placeholders::_25);
				break;
			}
			case 26: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24,
									 std::placeholders::_25,
									 std::placeholders::_26);
				break;
			}
			case 27: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24,
									 std::placeholders::_25,
									 std::placeholders::_26,
									 std::placeholders::_27);
				break;
			}
			case 28: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24,
									 std::placeholders::_25,
									 std::placeholders::_26,
									 std::placeholders::_27,
									 std::placeholders::_28);
				break;
			}
			case 29: {
				callback = std::bind(function, object, std::placeholders::_1,
									 std::placeholders::_2,
									 std::placeholders::_3,
									 std::placeholders::_4,
									 std::placeholders::_5,
									 std::placeholders::_6,
									 std::placeholders::_7,
									 std::placeholders::_8,
									 std::placeholders::_9,
									 std::placeholders::_10,
									 std::placeholders::_11,
									 std::placeholders::_12,
									 std::placeholders::_13,
									 std::placeholders::_14,
									 std::placeholders::_15,
									 std::placeholders::_16,
									 std::placeholders::_17,
									 std::placeholders::_18,
									 std::placeholders::_19,
									 std::placeholders::_20,
									 std::placeholders::_21,
									 std::placeholders::_22,
									 std::placeholders::_23,
									 std::placeholders::_24,
									 std::placeholders::_25,
									 std::placeholders::_26,
									 std::placeholders::_27,
									 std::placeholders::_28,
									 std::placeholders::_29);
				break;
			}
			default: {
				static_assert(false, "CMF: Too many arguments for an event handle callback.");
				break;
			}
		}
	}

	void operator () (Args&&... args) noexcept {
		callQueue.push(std::tuple<Args&&...>(args));
	}

	void scan(TickType_t wait = 0) noecept {
		if(!owningObject.isValid()){
			return;
		}

		if(callback != nullptr){
			return;
		}

		std::tuple<Args&&...> arguments;
		if(!callQueue.pop(arguments, wait)){
			return;
		}

		// TODO: this is terrible, I would prefer not to do this this way
		switch(sizeof...(Args)){
			case 0: {

				break;
			}
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
				static_assert(false, "CMF: Too many arguments for an event handle callback.");
				break;
			}
		}
	}

private:
	WeakObjectPtr<Object> owningObject; // Object of which the callback is a member, in case this owning object
	std::function<void(Args...)> callback;
	Queue<std::tuple<Args&&...>> callQueue;
};

#endif //CMF_EVENTHANDLE_H