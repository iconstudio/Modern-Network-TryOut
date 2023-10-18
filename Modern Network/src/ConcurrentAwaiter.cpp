module;
#include <thread>
module Net.Coroutine.Awaiter.Concurrent;

void
net::ConcurrentAwaiter::await_suspend(std::coroutine_handle<void> handle)
{
	std::thread([&handle] {
		handle();
	}).detach();
}