module;

export module Net.Coroutine;
import Net.Coroutine.Promissory;
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

			auto final_suspend() const noexcept
			{
				struct final_awaiter
				{
					static constexpr bool await_ready() noexcept { return false; }
					static constexpr void await_resume() noexcept {}

					std::coroutine_handle<void> await_suspend(handle_type h) noexcept
					{
						// final_awaiter::await_suspend is called when the execution of the
						// current coroutine (referred to by 'h') is about to finish.
						// If the current coroutine was resumed by another coroutine via
						// co_await get_task(), a handle to that coroutine has been stored
						// as h.promise().previous. In that case, return the handle to resume
						// the previous coroutine.
						// Otherwise, return noop_coroutine(), whose resumption does nothing.

						if (auto previous = h.promise().previous; previous)
							return previous;
						else
							return std::noop_coroutine();
					}
				};

				return final_awaiter{};
			}

			[[noreturn]]
			void unhandled_exception()
			{
				throw;
			}

			std::coroutine_handle<void> previous;
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

		static constexpr bool await_ready() noexcept { return false; }
		static constexpr void await_resume() noexcept { }
		handle_type await_suspend(std::coroutine_handle<void> h) noexcept
		{
			myHandle.promise().previous = h;
			return myHandle;
		}

		void resume() const
		{
			if (myHandle && not myHandle.done())
			{
				myHandle.resume();
			}
		}

		void operator()() const
		{
			if (myHandle && not myHandle.done())
			{
				myHandle();
			}
		}

		[[nodiscard]]
		bool done() const noexcept
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
