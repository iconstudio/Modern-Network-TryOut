module;
#include <exception>
#include <thread>

module Net.Task;
import Net.Constraints;

template<typename T>
bool
net::TaskEngine<T>::await_ready()
const noexcept
{
	return false;
}

template<typename T>
void
net::TaskEngine<T>::await_suspend(std::coroutine_handle<> handle)
const
{
	std::thread([this, &handle] {
		valueHandle.wait();
		handle();
	}).detach();
}

template<typename T>
T
net::TaskEngine<T>::await_resume()
{
	return valueHandle.get();
}

template<typename T>
net::Task<T>
net::TaskPromise<T>::get_return_object()
noexcept
{
	return Task{ net::Task<T>::handle_type::from_promise(*this) };
}

template<typename T>
template<typename U>
void
net::TaskPromise<T>::return_value(U&& value)
{
	if constexpr (same_as<T, void>)
	{
		myHandle.set_value();
	}
	else
	{
		myHandle.set_value(std::forward<U>(value));
		static_assert(not convertible_to<T, U>);
	}
}

template<typename T>
net::TaskEngine<T>
net::TaskPromise<T>::initial_suspend()
noexcept
{
	return TaskEngine{ myHandle.get_future() };
}

template<typename T>
void
net::TaskPromise<T>::unhandled_exception()
{
	myHandle.set_exception_at_thread_exit(std::current_exception());
}
