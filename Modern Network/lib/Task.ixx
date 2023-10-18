module;
#include <stdexcept>
#include <thread>
#include <future>

export module Net.Task;
import <coroutine>;

export namespace net
{
	struct ConcurrentAwaiter
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		void await_suspend(std::coroutine_handle<void> handle)
		{
			std::thread([&handle] {
				handle();
			}).detach();
		}

		static constexpr void await_resume() noexcept
		{}
	};

	template<typename T>
	class Task final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct Awaiter
		{
			bool await_ready() const noexcept
			{
				return true;
			}

			void await_suspend(std::coroutine_handle<void> handle) const noexcept
			{
				std::thread([this, handle] {
					valueHandle.wait();
					handle();
				}).detach();
			}

			const T& await_resume() const noexcept
			{
				return valueHandle.get();
			}

			std::shared_future<T> valueHandle;
		};

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
			std::future<T> myValueHandle;
		};

		constexpr Task(const handle_type& handle, const std::shared_future<T>& future) noexcept
			: myHandle(handle), valueHandle(future)
		{}

		constexpr Task(const handle_type& handle, std::shared_future<T>&& future) noexcept
			: myHandle(handle), valueHandle(std::move(future))
		{}

		constexpr Task(handle_type&& handle, const std::shared_future<T>& future) noexcept
			: myHandle(std::move(handle)), valueHandle(future)
		{}

		constexpr Task(handle_type&& handle, std::shared_future<T>&& future) noexcept
			: myHandle(std::move(handle)), valueHandle(std::move(future))
		{}

		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
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
		Awaiter operator co_await() & noexcept
		{
			return Awaiter{ valueHandle };
		}

		[[nodiscard]]
		Awaiter operator co_await() && noexcept
		{
			return Awaiter{ std::move(valueHandle) };
		}

		[[nodiscard]]
		T Result() const
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
		std::shared_future<T> valueHandle;
	};
}
