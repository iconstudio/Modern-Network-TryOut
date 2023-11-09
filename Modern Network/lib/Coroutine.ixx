export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Awaitable;
export import <coroutine>;

export namespace net::coroutine
{
	class Coroutine final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
		public:
			[[nodiscard]]
			Coroutine get_return_object() noexcept
			{
				return Coroutine(handle_type::from_promise(*this));
			}

			static constexpr std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			static constexpr std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			static constexpr void return_void() noexcept
			{}

			[[noreturn]]
			static void unhandled_exception()
			{
				throw;
			}
		};

		constexpr Coroutine(const handle_type& handle) noexcept
			: myHandle(handle)
		{}

		constexpr Coroutine(handle_type&& handle) noexcept
			: myHandle(static_cast<handle_type&&>(handle))
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
				myHandle.resume();
			}
		}

		void operator()() const
		{
			if (myHandle)
			{
				myHandle();
			}
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	private:
		handle_type myHandle;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
}
