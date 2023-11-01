module;
#include <thread>
module Net.Scheduler;
net::coroutine::Schedule::Schedule(net::coroutine::Schedule::handle_type handle, std::jthread&& worker)
	: Handler(handle), coWorker(std::move(worker))
noexcept
{}

net::coroutine::Schedule::~Schedule()
noexcept
{
	coWorker.request_stop();
}
