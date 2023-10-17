module;
#define _RESUMABLE_FUNCTIONS_SUPPORTED
export module Net.Task;
import Net.Constraints;
import Net.Coroutine.Suspender;
import Net.Coroutine.Promissory;
import Net.Coroutine.IPromise;
import <exception>;
import <stdexcept>;
import <coroutine>;
import <thread>;
import <future>;

export namespace net
{
	template<typename T = void>
	class Task;

	template<>
	class Task<void> final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			[[nodiscard]]
			Task<void> get_return_object() noexcept
			{
				return Task{ handle_type::from_promise(*this) };
			}

			[[nodiscard]]
			static constexpr std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			[[nodiscard]]
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
		static_assert(not Promissory<promise_type>);

		constexpr Task(const handle_type& handle) noexcept
			: myHandle(handle)
		{}
		constexpr Task(handle_type&& handle) noexcept
			: myHandle(std::move(handle))
		{}
		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		void operator()() const
		{
			myHandle();
		}
		void Resume() const
		{
			myHandle.resume();
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		[[nodiscard]]
		constexpr bool operator==(const Task&) const noexcept = default;

	private:
		handle_type myHandle;
	};

	template<notvoids T>
	class [[nodiscard]] Task<T> final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct TaskEngine
		{
			bool await_ready() const noexcept
			{
				return false;
			}

			void await_suspend(handle_type handle) const noexcept
			{
				std::thread([this, &handle] {
					valueHandle.wait();
					handle();
				}).detach();
			}

			[[nodiscard]]
			T await_resume()
			{
				return valueHandle.get();
			}

			std::future<T> valueHandle;
		};

		struct promise_type
		{
			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				return Task{ handle_type::from_promise(*this) };
			}

			template<convertible_to<T> U>
			constexpr void return_value(U&& value)
				noexcept(nothrow_assignable<T, U&&>)
			{
				myHandle.set_value(std::forward<U>(value));
			}

			[[nodiscard]]
			TaskEngine initial_suspend() noexcept
			{
				return TaskEngine{ myHandle.get_future() };
			}

			static constexpr std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			[[noreturn]]
			void unhandled_exception()
			{
				myHandle.set_exception_at_thread_exit(std::current_exception());
			}

			std::promise<T> myHandle;
		};
		static_assert(not Promissory<promise_type>);

		constexpr Task(const handle_type& handle) noexcept
			: myHandle(handle)
			, reservedError("Cannot acquire a vale from the null promise")
		{}
		constexpr Task(handle_type&& handle) noexcept
			: myHandle(std::move(handle))
			, reservedError("Cannot acquire a vale from the null promise")
		{}
		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		void Resume() const
		{
			if (myHandle && !myHandle.done())
			{
				myHandle.resume();
			}
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
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		handle_type myHandle;
		std::runtime_error reservedError;
	};
}
