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
#include "Util/stdafx.h"

template<typename ...Args>
struct BindHelper {
	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept {
		// TODO: log warning
		return nullptr;
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 0) {
		return std::bind(function, object);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 1) {
		return std::bind(function, object, std::placeholders::_1);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 2) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 3) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 4) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 5) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4,
						 std::placeholders::_5);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 6) {
		return std::bind(function, object, std::placeholders::_1,
									std::placeholders::_2,
									std::placeholders::_3,
									std::placeholders::_4,
									std::placeholders::_5,
									std::placeholders::_6);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 7) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4,
						 std::placeholders::_5,
						 std::placeholders::_6,
						 std::placeholders::_7);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 8) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4,
						 std::placeholders::_5,
						 std::placeholders::_6,
						 std::placeholders::_7,
						 std::placeholders::_8);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 9) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4,
						 std::placeholders::_5,
						 std::placeholders::_6,
						 std::placeholders::_7,
						 std::placeholders::_8,
						 std::placeholders::_9);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 10) {
		return std::bind(function, object, std::placeholders::_1,
						 std::placeholders::_2,
						 std::placeholders::_3,
						 std::placeholders::_4,
						 std::placeholders::_5,
						 std::placeholders::_6,
						 std::placeholders::_7,
						 std::placeholders::_8,
						 std::placeholders::_9,
						 std::placeholders::_10);
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 11) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 12) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 13) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 14) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 15) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 16) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 17) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 18) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 19) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 20) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 21) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 22) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 23) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 24) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 25) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 26) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 27) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 28) {
		return std::bind(function, object, std::placeholders::_1,
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
	}

	template<typename O, typename F>
	inline static constexpr std::function<void(Args...)> get(O* object, F&& function) noexcept requires (sizeof...(Args) == 29) {
		return std::bind(function, object, std::placeholders::_1,
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
	}
};

template<typename ...Args>
struct CallHelper {
	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept {
		// TODO log warning
		return false;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 0) {
		if(function == nullptr){
			return false;
		}

		function();

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 1) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 2) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 3) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 4) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 5) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 6) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 7) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 8) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 9) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 10) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 11) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 12) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 13) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 14) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 15) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 16) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 17) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 18) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 19) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 20) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 21) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 22) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 23) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 24) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 25) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments),
				 std::get<24>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 26) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments),
				 std::get<24>(arguments),
				 std::get<25>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 27) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments),
				 std::get<24>(arguments),
				 std::get<25>(arguments),
				 std::get<26>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 28) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments),
				 std::get<24>(arguments),
				 std::get<25>(arguments),
				 std::get<26>(arguments),
				 std::get<27>(arguments));

		return true;
	}

	inline static constexpr bool call(const std::function<void(Args...)>& function, const std::tuple<Args...>& arguments) noexcept requires (sizeof...(Args) == 29) {
		if(function == nullptr){
			return false;
		}

		function(std::get<0>(arguments),
				 std::get<1>(arguments),
				 std::get<2>(arguments),
				 std::get<3>(arguments),
				 std::get<4>(arguments),
				 std::get<5>(arguments),
				 std::get<6>(arguments),
				 std::get<7>(arguments),
				 std::get<8>(arguments),
				 std::get<9>(arguments),
				 std::get<10>(arguments),
				 std::get<11>(arguments),
				 std::get<12>(arguments),
				 std::get<13>(arguments),
				 std::get<14>(arguments),
				 std::get<15>(arguments),
				 std::get<16>(arguments),
				 std::get<17>(arguments),
				 std::get<18>(arguments),
				 std::get<19>(arguments),
				 std::get<20>(arguments),
				 std::get<21>(arguments),
				 std::get<22>(arguments),
				 std::get<23>(arguments),
				 std::get<24>(arguments),
				 std::get<25>(arguments),
				 std::get<26>(arguments),
				 std::get<27>(arguments),
				 std::get<28>(arguments));

		return true;
	}

private:
	inline CallHelper() noexcept = delete;
};

class EventHandleBase {
public:
	inline virtual void scan(TickType_t wait) noexcept = 0;
};

template<typename ...Args>
class EventHandle : public EventHandleBase {
public:
	inline virtual ~EventHandle() noexcept {
		if(owningObject.isValid()){
			owningObject->unregisterEventHandle(this);
		}
	}

	template<typename O, typename F>
	inline EventHandle& bind(O* object, F&& function) noexcept {
		if(owningObject.isValid()){
			owningObject->unregisterEventHandle(this);
		}

		owningObject = cast<Object>(object);
		callback = BindHelper<Args...>::template get(object, function);

		if(owningObject.isValid()){
			owningObject->registerEventHandle(this);
		}

		return *this;
	}

	inline EventHandle& bind(Object* object, const std::function<void(Args...)>& function) noexcept {
		if(owningObject.isValid()){
			owningObject->unregisterEventHandle(this);
		}

		owningObject = object;
		callback = function;

		if(owningObject.isValid()){
			owningObject->registerEventHandle(this);
		}

		return *this;
	}

	inline bool call(TickType_t wait, Args&&... args) noexcept {
		return callQueue.push(std::tuple<Args...>(args...), wait);
	}

	inline virtual void scan(TickType_t wait) noexcept override {
		if(!owningObject.isValid()){
			return;
		}

		if(callback == nullptr){
			return;
		}

		while(!callQueue.empty()){
			const uint64_t beginTime = millis();

			std::tuple<Args...> arguments;
			if(!callQueue.pop(arguments, wait)){
				break;
			}

			CallHelper<Args...>::call(callback, arguments);

			wait = std::max((uint64_t)0, (uint64_t) (wait - (millis() - beginTime)));
		}
	}

private:
	WeakObjectPtr<Object> owningObject = nullptr; // Object of which the callback is a member, in case this owning object
	std::function<void(Args...)> callback = nullptr;
	Queue<std::tuple<Args...>> callQueue;
};

#endif //CMF_EVENTHANDLE_H