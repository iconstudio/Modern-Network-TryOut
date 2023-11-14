module;
#include <Windows.h>

export module Net.Coroutine:TimedAwaiter;
import <atomic>;
import <ratio>;
import <thread>;
import <coroutine>;

export namespace net::coroutine
{
	struct [[nodiscard]] WaitForSeconds final
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void> h)
		{
			if (h and not h.done())
			{
				std::thread{ [this, h] {
					if (0 < myTime)
					{
						//std::chrono::milliseconds time = std::chrono::milliseconds((long long)(myTime * 1000));
						//std::this_thread::sleep_for(time);
						::Sleep(static_cast<DWORD>(myTime * 1000));
					}

					//myEvent.notify_one();
					h();
				} }.detach();
			}
		}
		static constexpr void await_resume() noexcept
		{}

		float myTime;
	};

	class [[nodiscard]] WaitForMilliseconds final
	{
	public:
		constexpr WaitForMilliseconds(float time) noexcept
			: myTime(time)
		{}

		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void> handle) const;
		constexpr float await_resume() const noexcept
		{
			return myTime;
		}

		float myTime;
	};

	class [[nodiscard]] WaitForMinutes final
	{
	public:
		constexpr WaitForMinutes(float time) noexcept
			: myTime(time)
		{}

		static constexpr bool await_ready() noexcept
		{
			return false;
		}
		void await_suspend(std::coroutine_handle<void> handle) const;
		constexpr float await_resume() const noexcept
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
