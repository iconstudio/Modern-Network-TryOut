export module Net.Coroutine.Awaiter.Timed;
import <coroutine>;

export namespace net
{
	struct WaitForSeconds
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<> handle) const;

		static constexpr void await_resume() noexcept
		{}

		float seconds;
	};
}
