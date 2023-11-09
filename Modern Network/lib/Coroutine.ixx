export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Awaitable;
export import <coroutine>;

export namespace net::coroutine
{
	class Coroutine;

	template<typename Co, Awaitable Initializer, Awaitable Finalizer>
	class Promise
	{
	public:
		[[nodiscard]]
		Co get_return_object() noexcept
		{
			return Co(Co::handle_type::from_promise(*this));
		}

		constexpr Initializer initial_suspend()
			const noexcept(nothrow_default_constructibles<Initializer>)
		{
			return {};
		}

		constexpr Finalizer final_suspend() const noexcept(nothrow_constructible<Finalizer>)
		{
			return Finalizer();
		}

		static constexpr void return_void() noexcept
		{}

		[[noreturn]]
		static void unhandled_exception()
		{
			throw;
		}
	};

	template<Awaitable Initializer = std::suspend_never, Awaitable Finalizer = std::suspend_always>
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

			constexpr Initializer initial_suspend() const noexcept(nothrow_default_constructibles<Initializer>)
			{
				return {};
			}

			constexpr Finalizer final_suspend() const noexcept(nothrow_default_constructibles<Finalizer>)
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

		void resume() const
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
