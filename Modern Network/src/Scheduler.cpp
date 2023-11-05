module;
#include <thread>
#include <memory>
#include <optional>
#include <algorithm>
module Net.Scheduler;

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
	: myScheduler(scheduler), myStarter(nullptr)
	, isSucceed(false)
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

	return not out;
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
			try
			{
				//if (not schedule->IsBusy())
				{
					schedule->AddTask(handle);
					myStarter = schedule.get();
				}
			}
			catch (...)
			{
				isSucceed = false;
				myStarter = nullptr;
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

std::optional<net::coroutine::Schedule* const>
net::coroutine::Scheduler::Initiator::await_resume()
const noexcept
{
	if (isSucceed)
	{
		return myStarter;
	}
	else
	{
		return std::nullopt;
	}
}
