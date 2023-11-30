export module Net.Io.Schedule;
import Net.Io.Event;
import Net.Coroutine;
import <coroutine>;

export namespace net::io
{
	struct Scheduler
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		std::coroutine_handle<void> await_suspend(std::coroutine_handle<void> previous_frame) noexcept;
		void await_resume() noexcept;
	};

	class [[nodiscard]] Schedule
	{
	private:

	public:
		Scheduler operator co_await() noexcept;
	};
}
