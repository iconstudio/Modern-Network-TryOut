module;
#include <vector>
#include <memory>
#include <thread>

export module Net.Scheduler;
import Net.Handler;
import Net.Coroutine;
import <optional>;

export namespace net::coroutine
{
	struct [[nodiscard]] Schedule final : public Handler<std::coroutine_handle<void>>
	{
		using super = Handler<std::coroutine_handle<void>>;
		using handle_type = std::coroutine_handle<void>;

		using super::super;
		~Schedule() noexcept = default;

		[[nodiscard]] std::suspend_always Pause() const noexcept;
		void Resume() const noexcept;

		Schedule(Schedule&&) noexcept = default;
		Schedule& operator=(Schedule&&) noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		void operator=(const Schedule&) = delete;
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
			/// Retrieves the managed schedule as successfully this task is queued on the scheduler.
			/// </summary>
			/// <returns>A managed schedule or empty if is failed</returns>
			std::optional<Schedule*> await_resume() const noexcept;

		private:
			Scheduler& myScheduler;
			Schedule* mySchedule;
			bool isSucceed;
		};

	public:
		Scheduler();
		Scheduler(size_t pipelines);

		[[nodiscard]] Initiator Start();

	protected:
		std::vector<std::unique_ptr<Schedule>> myWorkers;
	};
}

export namespace net
{
	using coroutine::Scheduler;
}

