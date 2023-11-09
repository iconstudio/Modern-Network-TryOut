export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Awaitable;
import <thread>;
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

	class AsyncCoroutine final
	{
	private:
		struct Initializer;

	public:
		class promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		class promise_type
		{
		public:
			[[nodiscard]]
			AsyncCoroutine get_return_object() noexcept
			{
				AsyncCoroutine co = AsyncCoroutine(handle_type::from_promise(*this));
				coHandle = co.myHandle;

				return co;
			}

			Initializer initial_suspend() const noexcept(nothrow_default_constructibles<Initializer, handle_type>)
			{
				return { coHandle };
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

			handle_type coHandle;
		};

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
		struct Initializer
		{
		public:
			static constexpr bool await_ready() noexcept
			{
				return false;
			}

			void await_suspend(std::coroutine_handle<void>) const noexcept
			{
				std::thread([this] {
					coHandle();
				}).detach();
			}

			static constexpr void await_resume() noexcept
			{}

			handle_type coHandle;
		};

		handle_type myHandle;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
	using AsyncCoroutine = coroutine::AsyncCoroutine;
}
