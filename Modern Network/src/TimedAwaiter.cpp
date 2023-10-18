module;
#include <Windows.h>
#include <chrono>
#include <thread>
module Net.Coroutine.Awaiter.Timed;

void
net::WaitForSeconds::await_suspend(std::coroutine_handle<> handle)
const
{
	//using namespace std::chrono_literals;

	//std::chrono::milliseconds time = std::chrono::milliseconds((long long)(seconds * 1000));
	std::thread([this, handle] {
		::Sleep(seconds * 1000);
		//std::this_thread::sleep_for(time);
		handle();
	}).detach();
}
