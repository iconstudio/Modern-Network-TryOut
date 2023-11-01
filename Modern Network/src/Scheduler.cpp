module;
#include <thread>
#include <memory>
module Net.Scheduler;

net::coroutine::Scheduler::Scheduler()
	: Scheduler(std::thread::hardware_concurrency())
{}

net::coroutine::Scheduler::Scheduler(size_t pipelines)
	: myWorkers(pipelines)
{}

net::coroutine::Scheduler::Initiator
net::coroutine::Scheduler::Start()
{
	return Initiator{ *this };
}

net::coroutine::Schedule::Schedule(handle_type handle, Scheduler& scheduler) noexcept
	: Handler(handle), myParent(std::addressof(scheduler))
	, isPaused(false)
{}

std::suspend_never
net::coroutine::Schedule::Pause()
noexcept
{
	bool paused = not isPaused.load(std::memory_order_acquire);
	if (not paused) [[likely]] {
		paused = true;
		std::this_thread::yield();

		isPaused.wait(false, std::memory_order_release);
	}
	else
	{
		isPaused.store(paused, std::memory_order_release);
		std::this_thread::yield();
	}

	return {};
}

void
net::coroutine::Schedule::Resume()
noexcept
{
	bool expected = true;

	if (myHandle and isPaused.compare_exchange_strong(expected, false, std::memory_order_acq_rel))
	{
		isPaused.notify_one();

		myHandle.resume();
	}
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
		auto schedule = std::make_unique<coroutine::Schedule>(handle, std::ref(myScheduler));
		mySchedule = schedule.get();

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
		mySchedule = nullptr;
		isSucceed = false;
	}
}

std::optional<net::coroutine::Schedule*>
net::coroutine::Scheduler::Initiator::await_resume()
const noexcept
{
	if (isSucceed)
	{
		return mySchedule;
	}
	else
	{
		return std::nullopt;
	}
}
