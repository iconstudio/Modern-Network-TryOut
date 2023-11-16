export module Net.Coroutine;
export import :TimedAwaiter;
import <atomic>;
export import <coroutine>;

export namespace net::coroutine
{
	using std::suspend_never;
	using std::suspend_always;
	using std::coroutine_handle;

	class Coroutine final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			[[nodiscard]]
			Coroutine get_return_object() noexcept
			{
				return Coroutine(handle_type::from_promise(*this));
			}

			static constexpr void return_void() noexcept
			{}

			static constexpr std::suspend_never initial_suspend() noexcept
			{
				return {};
			}

			static constexpr std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			[[noreturn]]
			static void unhandled_exception()
			{
				throw;
			}
		};

		constexpr Coroutine(const handle_type& handle) noexcept
			: myHandle(handle), isTriggered()
		{}

		constexpr Coroutine(handle_type&& handle) noexcept
			: myHandle(static_cast<handle_type&&>(handle)), isTriggered()
		{}

		~Coroutine() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		void Resume() const
		{
			if (myHandle)
			{
				isTriggered = true;

				myHandle.resume();
			}
		}

		void operator()() const
		{
			if (myHandle)
			{
				isTriggered = true;

				myHandle();
			}
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			if (IsTriggered())
			{
				return myHandle.done();
			}
			else
			{
				return false;
			}
		}

		[[nodiscard]]
		bool IsTriggered() const noexcept
		{
			return isTriggered.load(std::memory_order_relaxed);
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	private:
		handle_type myHandle;
		volatile std::atomic_bool isTriggered;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
}
