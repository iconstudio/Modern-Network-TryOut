module;
#include <Windows.h>
#include <chrono>
#include <thread>
module Net.Coroutine.Awaiter.Timed;

void
net::coroutine::WaitForSeconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	//using namespace std::chrono_literals;

	//std::chrono::milliseconds time = std::chrono::milliseconds((long long)(myTime * 1000));
	//std::this_thread::sleep_for(time);

	std::thread{ [this, &handle] {
		::Sleep(static_cast<DWORD>(myTime * 1000));
		if (handle and not handle.done())
		{
			handle();
		}
	} }.detach();
}

void
net::coroutine::WaitForMilliseconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	std::thread{ [this, &handle] {
		::Sleep(static_cast<DWORD>(myTime));
		if (handle and not handle.done())
		{
			handle();
		}
	} }.detach();
}

void
net::coroutine::WaitForMinutes::await_suspend(std::coroutine_handle<void> handle)
const
{
	std::thread{ [this, &handle] {
		::Sleep(static_cast<DWORD>(myTime * 60000));
		if (handle and not handle.done())
		{
			handle();
		}
	} }.detach();
}

void
net::coroutine::SynchronousWaitForSeconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	::Sleep(static_cast<DWORD>(myTime * 1000));
	handle();
}

void
net::coroutine::SynchronousWaitForMilliseconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	::Sleep(static_cast<DWORD>(myTime));
	handle();
}

void
net::coroutine::SynchronousWaitForMinutes::await_suspend(std::coroutine_handle<void> handle)
const
{
	::Sleep(static_cast<DWORD>(myTime * 60000));
	handle();
}
