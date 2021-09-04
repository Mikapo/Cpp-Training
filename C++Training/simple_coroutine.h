#pragma once

#include <coroutine>
#include <iostream>
#include <future>


template <typename T>
struct future_awaiter
{
	std::future<T> future;

	bool await_ready() { return future._Is_ready(); };

	void await_suspend(std::coroutine_handle<> ch)
	{
		future.wait();
		ch.resume();
	}

	auto await_resume() { return future.get(); };


};

template<typename T, typename... arguments>
struct std::coroutine_traits<std::future<T>, arguments...>
{
	struct promise_type
	{
		promise<T> _promise;

		future<T> get_return_object() { return _promise.get_future(); };

		auto inital_suspend() { return std::suspend_never; };
		auto final_suspend() { return std::suspend_never; };

		template<typename U>
		void return_value(U&& value)
		{
			_promise.set_value(std::forward<U>(value));
		}

		void set_expection(std::exception_ptr ex) { _promise.set_exception(std::move(ex)); };
	};
};

class simple_coroutine
{

public:

	void resume_coroutine();
	void run_async(int number);

private:

	static int async_function(int number);

	std::future<int> Coroutine();
	std::future<int> return_value;

	
	
};

