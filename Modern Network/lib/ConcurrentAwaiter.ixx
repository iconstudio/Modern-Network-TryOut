export module Net.Coroutine.Awaiter.Concurrent;
import <coroutine>;

export namespace net
{
	struct [[nodiscard]] ConcurrentAwaiter
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle);

		static constexpr void await_resume() noexcept
		{}
	};
}
