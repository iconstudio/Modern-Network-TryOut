module;
#include <stdexcept>
#include <future>

export module Net.Task;
import Net.Constraints;
import Net.Coroutine.Awaiter.Concurrent;
import <coroutine>;

export namespace net
{
	template<typename T = void>
	class Task;

	template<notvoids T>
	class Task<T> final
	{
	public:
		struct promise_type;
		struct finalizer;
		using handle_type = std::coroutine_handle<promise_type>;
		using promise_handle_type = std::promise<T>;
		using future_type = std::future<T>;
		using public_future_type = std::shared_future<T>;

		struct promise_type
		{
			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				myValueHandle = myHandle.get_future();

				return Task(handle_type::from_promise(*this), myValueHandle.share());
			}

			template<typename U>
			void return_value(U&& value)
			{
				myHandle.set_value(std::forward<U>(value));
			}

			ConcurrentAwaiter initial_suspend() noexcept
			{
				return {};
			}

			finalizer final_suspend() const noexcept
			{
				return {};
			}

			[[noreturn]]
			void unhandled_exception()
			{
				myHandle.set_exception_at_thread_exit(std::current_exception());
			}

			promise_handle_type myHandle;
			std::coroutine_handle<void> contextHandle;
			future_type myValueHandle;
		};

		struct finalizer
		{
			static constexpr bool await_ready() noexcept { return false; }
			static constexpr void await_resume() noexcept {}

			void await_suspend(handle_type handle) const noexcept
			{
				promise_type& my_promise = handle.promise();
				if (std::coroutine_handle<void> previous = my_promise.contextHandle; previous)
				{
					previous();
				}
			}
		};

		Task(const handle_type& handle, const public_future_type& future) noexcept
			: myHandle(handle), valueHandle(future)
		{}

		Task(const handle_type& handle, public_future_type&& future) noexcept
			: myHandle(handle), valueHandle(std::move(future))
		{}

		Task(handle_type&& handle, const public_future_type& future) noexcept
			: myHandle(std::move(handle)), valueHandle(future)
		{}

		Task(handle_type&& handle, public_future_type&& future) noexcept
			: myHandle(std::move(handle)), valueHandle(std::move(future))
		{}

		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		static constexpr bool await_ready() noexcept
		{
			return true;
		}

		void await_suspend(std::coroutine_handle<void> handle) const noexcept
		{
			promise_type& my_promise = myHandle.promise();
			my_promise.contextHandle = myHandle;

			valueHandle.wait();
			handle();
		}

		const T& await_resume() const
		{
			return valueHandle.get();
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		void operator()() const
		{
			if (myHandle && !myHandle.done())
			{
				myHandle();
			}
		}

		[[nodiscard]]
		const T& Result() const
		{
			return valueHandle.get();
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
		public_future_type valueHandle;
	};

	template<>
	class Task<void> final
	{
	public:
		struct promise_type;
		struct finalizer;
		using handle_type = std::coroutine_handle<promise_type>;
		using promise_handle_type = std::promise<void>;
		using future_type = std::future<void>;
		using public_future_type = std::shared_future<void>;

		struct promise_type
		{
			[[nodiscard]]
			Task<void> get_return_object() noexcept
			{
				myValueHandle = myHandle.get_future();

				return Task(handle_type::from_promise(*this), myValueHandle.share());
			}

			void return_void()
			{
				myHandle.set_value();
			}

			ConcurrentAwaiter initial_suspend() noexcept
			{
				return {};
			}

			finalizer final_suspend() const noexcept
			{
				return {};
			}

			[[noreturn]]
			void unhandled_exception()
			{
				myHandle.set_exception_at_thread_exit(std::current_exception());
			}

			promise_handle_type myHandle;
			std::coroutine_handle<void> contextHandle;
			future_type myValueHandle;
		};

		struct finalizer
		{
			static constexpr bool await_ready() noexcept { return false; }
			static constexpr void await_resume() noexcept {}

			void await_suspend(handle_type handle) const noexcept
			{
				promise_type& my_promise = handle.promise();
				if (std::coroutine_handle<void> previous = my_promise.contextHandle; previous)
				{
					previous();
				}
			}
		};

		Task(const handle_type& handle, const public_future_type& future) noexcept
			: myHandle(handle), valueHandle(future)
		{}

		Task(const handle_type& handle, public_future_type&& future) noexcept
			: myHandle(handle), valueHandle(std::move(future))
		{}

		Task(handle_type&& handle, const public_future_type& future) noexcept
			: myHandle(std::move(handle)), valueHandle(future)
		{}

		Task(handle_type&& handle, public_future_type&& future) noexcept
			: myHandle(std::move(handle)), valueHandle(std::move(future))
		{}

		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		static constexpr bool await_ready() noexcept
		{
			return true;
		}

		void await_suspend(std::coroutine_handle<void> handle) const noexcept
		{
			promise_type& my_promise = myHandle.promise();
			my_promise.contextHandle = myHandle;

			valueHandle.wait();
			handle();
		}

		void await_resume() const noexcept
		{}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		void operator()() const
		{
			if (myHandle && !myHandle.done())
			{
				myHandle();
			}
		}

		void Result() const
		{
			return valueHandle.get();
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
		public_future_type valueHandle;
	};
}
