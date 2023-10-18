export module Net.Coroutine.Awaiter.Timed;
import <coroutine>;

export namespace net
{
	class WaitForSeconds
	{
	public:
		constexpr WaitForSeconds(const float& wait_seconds) noexcept
			: seconds(wait_seconds)
		{}

		constexpr WaitForSeconds(float&& wait_seconds) noexcept
			: seconds(static_cast<float&&>(wait_seconds))
		{}

		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<> handle) const;

		constexpr float await_resume() const noexcept
		{
			return seconds;
		}

		float seconds;
	};
}
