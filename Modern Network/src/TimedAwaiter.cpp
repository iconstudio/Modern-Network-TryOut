module;
#include <Windows.h>
#include <chrono>
#include <thread>

module Net.Coroutine;
import :TimedAwaiter;

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
net::coroutine::SynchronousWaitForSeconds::await_resume()
const noexcept
{
	::Sleep(static_cast<DWORD>(myTime * 1000));
}

void
net::coroutine::SynchronousWaitForMilliseconds::await_resume()
const noexcept
{
	::Sleep(static_cast<DWORD>(myTime));
}

void
net::coroutine::SynchronousWaitForMinutes::await_resume()
const noexcept
{
	::Sleep(static_cast<DWORD>(myTime * 60000));
}
