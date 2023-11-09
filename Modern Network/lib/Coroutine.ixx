export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Awaitable;
export import <coroutine>;

export namespace net::coroutine
{
	class Coroutine;
	class AsyncCoroutine;

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
		using promise_type = Promise<Coroutine, std::suspend_never, std::suspend_always>;
		using handle_type = std::coroutine_handle<promise_type>;

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

	class AsyncCoroutine final
	{
	public:
		using promise_type = Promise<Coroutine, std::suspend_never, std::suspend_always>;
		using handle_type = std::coroutine_handle<promise_type>;

		constexpr AsyncCoroutine(const handle_type& handle) noexcept
			: myHandle(handle)
		{}

		constexpr AsyncCoroutine(handle_type&& handle) noexcept
			: myHandle(static_cast<handle_type&&>(handle))
		{}

		~AsyncCoroutine() noexcept(noexcept(myHandle.destroy()))
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
		constexpr bool operator==(const AsyncCoroutine&) const noexcept = default;

	private:
		handle_type myHandle;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
	using AsyncCoroutine = coroutine::AsyncCoroutine;
}
