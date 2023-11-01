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
	struct [[nodiscard]] Schedule final : public Handler<coroutine::Coroutine::handle_type>
	{
		using super = Handler<coroutine::Coroutine::handle_type>;
		using handle_type = coroutine::Coroutine::handle_type;

		Schedule(handle_type handle, std::jthread&& worker) noexcept;
		~Schedule() noexcept;

		[[nodiscard]] std::suspend_always Pause() const noexcept;
		void Resume() const noexcept;

		Schedule(Schedule&&) noexcept = default;
		Schedule& operator=(Schedule&&) noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		void operator=(const Schedule&) = delete;

		std::jthread coWorker;
	};

	class [[nodiscard]] Scheduler
	{
	protected:
		struct [[nodiscard]] ScheduleInitiator final
		{
			ScheduleInitiator(Scheduler&) noexcept;
			~ScheduleInitiator() noexcept = default;

			static constexpr bool await_ready() noexcept { return false; } // always suspends
			void await_suspend(std::coroutine_handle<void> handle) noexcept;
			/// <summary>
			/// Retrieves the managed schedule as successfully this task is queued on the scheduler.
			/// </summary>
			/// <returns>A managed schedule or empty if is failed</returns>
			std::optional<Schedule&> await_resume() const noexcept;

		private:
			bool __is_succeed = false;
		};

	public:
		Scheduler();
		Scheduler(size_t pipeline);

		ScheduleInitiator Start();

	protected:
		std::vector<std::unique_ptr<Schedule>> myWorkers;
	};
}

export namespace net
{
	using coroutine::Scheduler;
}

