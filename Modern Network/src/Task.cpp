module;
#include <exception>
#include <thread>

module Net.Task;
import Net.Constraints;

net::Task<void>
net::Task<void>::promise_type::get_return_object()
noexcept
{
	myValueHandle = myHandle.get_future();

	return Task(handle_type::from_promise(*this), myValueHandle.share());
}

void
net::Task<void>::promise_type::return_void()
{
	myHandle.set_value();
}

net::coroutine::ConcurrentAwaiter
net::Task<void>::promise_type::initial_suspend()
noexcept
{
	return {};
}

net::TaskFinalizer
net::Task<void>::promise_type::final_suspend()
const noexcept
{
	return {};
}

[[noreturn]]
void
net::Task<void>::promise_type::unhandled_exception()
{
	myHandle.set_exception_at_thread_exit(std::current_exception());
}
