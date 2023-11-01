module;
#include <vector>
#include <memory>
#include <optional>
#include <thread>

export module Net.Io.Scheduler;
import Net.Handler;
import Net.Coroutine;

export namespace net::io
{
	struct [[nodiscard]] Schedule final : public Handler<coroutine::Coroutine::handle_type>
	{
		using super = Handler<coroutine::Coroutine::handle_type>;
		using handle_type = coroutine::Coroutine::handle_type;

		using super::super;
		~Schedule() noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		void operator=(const Schedule&) = delete;

		std::jthread coWorker;
	};

	class [[nodiscard]] IoScheduler
	{
	protected:
		struct [[nodiscard]] ScheduleInitiator final
		{
			ScheduleInitiator(IoScheduler&) noexcept;
			~ScheduleInitiator() noexcept = default;

			static constexpr bool await_ready() noexcept { return false; } // always suspends
			void await_suspend(std::coroutine_handle<void> handle) noexcept;
			/// <summary>
			/// Retrieves how successfully this task is queued on the scheduler.
			/// </summary>
			/// <returns>Whether it has been scheduled or not</returns>
			bool await_resume() const noexcept;

		private:
			bool __is_succeed = false;
		};

	public:
		IoScheduler();
		IoScheduler(size_t pipeline);

		ScheduleInitiator Start();

	protected:
		std::vector<std::unique_ptr<Schedule>> myWorkers;
	};
}
