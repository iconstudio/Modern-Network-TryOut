export module Net.Coroutine;
import Net.Constraints;
export import :TimedAwaiter;
export import <coroutine>;

export namespace net::coroutine
{
	class Coroutine final
	{
	private:
		struct Awaiter;

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

			static constexpr std::suspend_never initial_suspend() noexcept
			{
				return {};
			}

			auto final_suspend() const noexcept
			{
				struct Finalizer
				{
					constexpr bool await_ready() const noexcept
					{
						return false;
					}

					std::coroutine_handle<void> await_suspend(handle_type current) noexcept
					{
						if (auto previous = current.promise().previousFrame; previous)
							return previous;
						else
							return std::noop_coroutine();
					}

					constexpr void await_resume() const noexcept
					{}
				};

				return Finalizer{};
			}

			static constexpr void return_void() noexcept
			{}

			[[noreturn]]
			static void unhandled_exception()
			{
				throw;
			}

			std::coroutine_handle<void> previousFrame;
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

		Awaiter operator co_await() const noexcept
		{
			return { myHandle };
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	private:
		struct Awaiter
		{
			static constexpr bool await_ready() noexcept { return false; }
			static constexpr void await_resume() noexcept {}

			handle_type await_suspend(std::coroutine_handle<void> previous_frame)
			{
				coHandle.promise().previousFrame = previous_frame;
				return coHandle;
			}

			std::coroutine_handle<promise_type> coHandle;
		};

		handle_type myHandle;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
}
