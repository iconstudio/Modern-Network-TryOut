export module Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.IAwaitable;
import <atomic>;
import <coroutine>;

export namespace net::coroutine
{
	class [[nodiscard]] WaitForSeconds final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void>);
		void await_resume() const noexcept;

		float myTime{};
		std::atomic_flag myEvent{};
	};

	class [[nodiscard]] WaitForMilliseconds final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void> handle) const;
		constexpr float await_resume() const noexcept
		{
			return myTime;
		}

		float myTime{};
	};

	class [[nodiscard]] WaitForMinutes final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void> handle) const;
		constexpr float await_resume() const noexcept
		{
			return myTime;
		}

		float myTime{};
	};

	class [[nodiscard]] SynchronousWaitForSeconds final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return true;
		}
		static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
		{}
		void await_resume() const noexcept;

		float myTime;
	};

	class [[nodiscard]] SynchronousWaitForMilliseconds final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return true;
		}
		static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
		{}
		void await_resume() const noexcept;

		float myTime;
	};

	class [[nodiscard]] SynchronousWaitForMinutes final
	{
	public:
		static constexpr bool await_ready() noexcept
		{
			return true;
		}
		static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
		{}
		void await_resume() const noexcept;

		float myTime;
	};
}
