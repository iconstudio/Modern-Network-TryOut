module;
#include <atomic>
#include <thread>
module Net.Scheduler;
import :Schedule;

net::coroutine::Schedule::Schedule(Scheduler& scheduler)
	: myWorker(), myTasks(), myParent(std::addressof(scheduler))
	, isPaused(false), isBusy(false), isLocked(false)
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
				Unlock();

				if (first and not first.done())
				{
					isBusy = true;
					first.resume();
					isBusy = false;
				}
			}
		}
	} };
}

void
net::coroutine::Schedule::AddTask(std::coroutine_handle<void> handle)
{
	Lock();
	myTasks.push_back(handle);
	Unlock();
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
	isLocked.store(true);
}

bool
net::coroutine::Schedule::TryLock()
noexcept
{
	bool expected = false;
	return isBusy.compare_exchange_strong(expected, true, std::memory_order_acq_rel);
}

void
net::coroutine::Schedule::Unlock()
noexcept
{
	isLocked.store(false);
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

bool
net::coroutine::Schedule::IsLocked()
const noexcept
{
	return isLocked.load(std::memory_order_relaxed);
}
