module;
#include <thread>
#include <memory>
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

net::coroutine::Scheduler::Initiator::Initiator(net::coroutine::Scheduler& scheduler)
noexcept
	: myScheduler(scheduler), mySchedule(nullptr), isSucceed(false)
{}

bool
net::coroutine::Scheduler::Initiator::await_ready()
const noexcept
{
	auto& workers = myScheduler.myWorkers;
	if (workers.size() < workers.capacity())
	{
		// let mySchedule be empty
		return true;
	}

	// suspend
	return false;
}

void
net::coroutine::Scheduler::Initiator::await_suspend(std::coroutine_handle<void> handle)
noexcept
{
	try
	{
		auto& workers = myScheduler.myWorkers;
#if _DEBUG
		auto schedule = std::make_unique<coroutine::Schedule>(handle);

		workers.push_back(std::move(schedule));
#else // _DEBUG
		workers.push_back(std::make_unique<coroutine::Schedule>(handle));
#endif // !_DEBUG

		// resume now
		handle.resume();

		isSucceed = true;
	}
	catch (...)
	{
		isSucceed = false;
	}
}
