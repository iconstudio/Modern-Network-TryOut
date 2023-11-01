module;
#include <thread>
module Net.Scheduler;

std::suspend_always
net::coroutine::Schedule::Pause()
const noexcept
{
	std::this_thread::yield();

	return {};
}

void
net::coroutine::Schedule::Resume()
const noexcept
{
	myHandle.resume();
}
