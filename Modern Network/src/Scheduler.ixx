module;
#include <vector>
#include <deque>
#include <memory>
#include <thread>
#include <atomic>

export module Net.Scheduler;
import Net.Handler;
import Net.Coroutine;
import <optional>;

export namespace net::coroutine
{
	class Scheduler;

	struct [[nodiscard]] Schedule final
	{
		Schedule(Scheduler& scheduler);
		~Schedule() noexcept = default;

		void AddTask(std::coroutine_handle<void> handle);
		[[nodiscard]] std::suspend_never Pause() noexcept;
		void Resume() noexcept;
		bool Stop() noexcept;
		void Lock() noexcept;
		bool TryLock() noexcept;
		void Unlock() noexcept;

		[[nodiscard]] size_t NumberOfTasks() const noexcept;
		[[nodiscard]] bool IsBusy() const noexcept;

		Schedule(Schedule&&) noexcept = default;
		Schedule& operator=(Schedule&&) noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		void operator=(const Schedule&) = delete;

		std::jthread myWorker;
		std::deque<std::coroutine_handle<void>> myTasks;

		Scheduler* myParent;
		volatile std::atomic_bool isPaused;
		volatile std::atomic_bool isBusy;
	};

	class [[nodiscard]] Scheduler
	{
	protected:
		struct [[nodiscard]] Initiator final
		{
			Initiator(Scheduler& scheduler) noexcept;
			~Initiator() noexcept = default;

			bool await_ready() const noexcept;
			void await_suspend(std::coroutine_handle<void> handle) noexcept;
			/// <summary>
			/// Retrieves how successfully this task is queued on the scheduler.
			/// </summary>
			/// <returns>Whether it has been scheduled or not</returns>
			bool await_resume() const noexcept;

			friend struct coroutine::Schedule;

		private:
			Scheduler& myScheduler;
			bool isSucceed;
		};

		std::vector<std::unique_ptr<Schedule>> mySchedules;

	public:
		Scheduler();
		Scheduler(size_t pipelines);

		[[nodiscard]] Initiator Start();

		friend struct coroutine::Schedule;
	};
}

export namespace net
{
	using coroutine::Scheduler;
}

