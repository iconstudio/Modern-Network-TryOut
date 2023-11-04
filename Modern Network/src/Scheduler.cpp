module;
#include <thread>
#include <memory>
#include <algorithm>
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
				std::coroutine_handle<void> first = myTasks.front();
				myTasks.pop_front();

				if (first)
				{
					first();
				}
				Unlock();
			}
		}
	} };
}

void
net::coroutine::Schedule::AddTask(std::coroutine_handle<void> handle)
{
	myTasks.push_back(handle);
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

size_t
net::coroutine::Schedule::NumberOfTasks()
const noexcept
{
	return myTasks.size();
}

bool
net::coroutine::Schedule::IsBusy()
const noexcept
{
	return isBusy.load(std::memory_order_relaxed);
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
		mySchedules.push_back(std::make_unique<Schedule>(*this));
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
		if (not schedule->IsBusy())
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
		auto& schedules = myScheduler.mySchedules;
		std::ranges::sort(schedules, {}, [](std::unique_ptr<Schedule>& ptr) noexcept -> size_t {
			return ptr->NumberOfTasks();
		});

		for (auto& schedule : schedules)
		{
			schedule->Lock();
			try
			{
				if (not schedule->IsBusy())
				{
					schedule->AddTask(handle);
				}
				schedule->Unlock();
			}
			catch (...)
			{
				isSucceed = false;
				schedule->Unlock();
				break;
			}
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
