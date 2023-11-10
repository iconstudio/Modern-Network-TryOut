export module Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.IAwaitable;
import <coroutine>;

export namespace net::coroutine
{
	class [[nodiscard]] WaitForSeconds final : public IAwaitable<float>
	{
	public:
		constexpr WaitForSeconds(const float& time) noexcept
			: IAwaitable(), myTime(time)
		{}

		constexpr WaitForSeconds(float&& time) noexcept
			: IAwaitable(), myTime(static_cast<float&&>(time))
		{}

		constexpr bool await_ready() noexcept override
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle) const override;

		constexpr float await_resume() const noexcept override
		{
			return myTime;
		}

		float myTime;
	};

	class [[nodiscard]] WaitForMilliseconds final : public IAwaitable<float>
	{
	public:
		constexpr WaitForMilliseconds(const float& time) noexcept
			: IAwaitable(), myTime(time)
		{}

		constexpr WaitForMilliseconds(float&& time) noexcept
			: IAwaitable(), myTime(static_cast<float&&>(time))
		{}

		constexpr bool await_ready() noexcept override
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle) const override;

		constexpr float await_resume() const noexcept override
		{
			return myTime;
		}

		float myTime;
	};

	class [[nodiscard]] WaitForMinutes final : public IAwaitable<float>
	{
	public:
		constexpr WaitForMinutes(const float& time) noexcept
			: IAwaitable(), myTime(time)
		{}

		constexpr WaitForMinutes(float&& time) noexcept
			: IAwaitable(), myTime(static_cast<float&&>(time))
		{}

		constexpr bool await_ready() noexcept override
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle) const override;

		constexpr float await_resume() const noexcept override
		{
			return myTime;
		}

		float myTime;
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
