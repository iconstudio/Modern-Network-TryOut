export module Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.IAwaitable;
import <coroutine>;

export namespace net
{
	class WaitForSeconds : public IAwaitable<float>
	{
	public:
		constexpr WaitForSeconds(const float& wait_seconds) noexcept
			: IAwaitable(), seconds(wait_seconds)
		{}

		constexpr WaitForSeconds(float&& wait_seconds) noexcept
			: IAwaitable(), seconds(static_cast<float&&>(wait_seconds))
		{}

		constexpr bool await_ready() noexcept override
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle) const override;

		constexpr float await_resume() const noexcept override
		{
			return seconds;
		}

		float seconds;
	};
}
