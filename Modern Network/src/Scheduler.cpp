module;
#include <thread>
module Net.Scheduler;

net::coroutine::Schedule::Schedule(net::coroutine::Schedule::handle_type handle, std::jthread&& worker)
	: Handler(handle), coWorker(std::move(worker))
noexcept
{}

std::suspend_always
net::coroutine::Schedule::Pause()
const noexcept
{
	return {};
}

void
net::coroutine::Schedule::Resume()
const noexcept
{
	myHandle.resume();
}
