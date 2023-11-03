module;
#include <vector>
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

	struct [[nodiscard]] Schedule final : public Handler<std::coroutine_handle<void>>
	{
		using super = Handler<std::coroutine_handle<void>>;
		using handle_type = std::coroutine_handle<void>;

		Schedule(handle_type handle, Scheduler& scheduler) noexcept;
		~Schedule() noexcept = default;

		[[nodiscard]] std::suspend_never Pause() noexcept;
		void Resume() noexcept;

		Schedule(Schedule&&) noexcept = default;
		Schedule& operator=(Schedule&&) noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		void operator=(const Schedule&) = delete;

		Scheduler* myParent;
		volatile std::atomic_bool isPaused;
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

		private:
			Scheduler& myScheduler;
			Schedule* mySchedule;
			bool isSucceed;
		};

	public:
		Scheduler();
		Scheduler(size_t pipelines);

		[[nodiscard]] Initiator Start();

		friend struct coroutine::Schedule;

	protected:
		std::vector<std::unique_ptr<Schedule>> myWorkers;
	};
}

export namespace net
{
	using coroutine::Scheduler;
}

