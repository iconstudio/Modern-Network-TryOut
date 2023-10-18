module;
#include <Windows.h>
#include <chrono>
#include <thread>
module Net.Coroutine.Awaiter.Timed;

void
net::WaitForSeconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	//using namespace std::chrono_literals;

	//std::chrono::milliseconds time = std::chrono::milliseconds((long long)(myTime * 1000));
	//std::this_thread::sleep_for(time);

	::Sleep(static_cast<DWORD>(myTime * 1000));
	handle();
}

void
net::WaitForMilliseconds::await_suspend(std::coroutine_handle<void> handle)
const
{
	::Sleep(static_cast<DWORD>(myTime));
	handle();
}

void
net::WaitForMinutes::await_suspend(std::coroutine_handle<void> handle)
const
{
	::Sleep(static_cast<DWORD>(myTime * 60000));
	handle();
}
