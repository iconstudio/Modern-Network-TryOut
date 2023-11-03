module;
#include <thread>
#include <memory>
module Net.Scheduler;

net::coroutine::Schedule::Schedule(Scheduler& scheduler)
	: myWorker(), myTasks(), myParent(std::addressof(scheduler))
	, isPaused(false), isBusy(false)
{
	myWorker = std::jthread{ [&](std::stop_token&& token) {
		while (true)
		{
			if (token.stop_requested()) [[unlikely]] {
				break;
			};

			if (0 < myTasks.size())
			{
				Lock();

				auto& first = myTasks.front();
				first();

				myTasks.pop_front();
				Unlock();
			}
		}
	} };
}

std::suspend_never
net::coroutine::Schedule::Pause()
noexcept
{
	bool paused = not isPaused.load(std::memory_order_acquire);
	if (not paused) [[likely]] {
		paused = true;

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

	if (isPaused.compare_exchange_strong(expected, false, std::memory_order_acq_rel))
	{
		isPaused.notify_one();
	}
}

bool
net::coroutine::Schedule::Stop()
noexcept
{
	return myWorker.request_stop();
}

void
net::coroutine::Schedule::Lock()
noexcept
{
	isBusy.store(true, std::memory_order_acquire);
}

void
net::coroutine::Schedule::Unlock()
noexcept
{
	isBusy.store(false, std::memory_order_release);
}

net::coroutine::Scheduler::Scheduler()
	: Scheduler(std::thread::hardware_concurrency())
{}

net::coroutine::Scheduler::Scheduler(size_t pipelines)
	: mySchedules()
{
	mySchedules.reserve(pipelines);

	for (size_t i = 0; i < pipelines; ++i)
	{
		mySchedules[i] = std::make_unique<Schedule>(*this);
	}

	mySchedules.shrink_to_fit();
}

net::coroutine::Scheduler::Initiator
net::coroutine::Scheduler::Start()
{
	return Initiator{ *this };
}

net::coroutine::Scheduler::Initiator::Initiator(net::coroutine::Scheduler& scheduler)
noexcept
	: myScheduler(scheduler), isSucceed(false)
{}

bool
net::coroutine::Scheduler::Initiator::await_ready()
const noexcept
{
	bool out = false;
	for (auto& schedule : myScheduler.mySchedules)
	{
		if (not schedule->isBusy)
		{
			out = true;
		}
	}

	return out;
}

void
net::coroutine::Scheduler::Initiator::await_suspend(std::coroutine_handle<void> handle)
noexcept
{
	try
	{
		for (auto& schedule : myScheduler.mySchedules)
		{

		}

		isSucceed = true;
	}
	catch (...)
	{
		isSucceed = false;
	}
}

bool
net::coroutine::Scheduler::Initiator::await_resume()
const noexcept
{
	if (isSucceed)
	{
		return true;
	}
	else
	{
		return false;
	}
}
