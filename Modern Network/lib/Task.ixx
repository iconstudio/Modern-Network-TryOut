module;
#include <stdexcept>
#include <thread>
#include <future>

export module Net.Task;
import <coroutine>;

export namespace net
{
	template<typename T>
	class [[nodiscard]] Task final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct Awaiter
		{
			bool await_ready() const noexcept
			{
				return false;
			}

			void await_suspend(handle_type handle) const
			{
				std::thread([this, handle] {
					valueHandle.wait();
					handle();
				}).detach();
			}

			T await_resume()
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
				return Task(handle_type::from_promise(*this));
			}

			[[nodiscard]]
			std::future<T> get_future()
			{
				return myHandle.get_future();
			}

			template<typename U>
			void return_value(U&& value)
			{
				myHandle.set_value(std::forward<U>(value));
			}

			static constexpr std::suspend_always initial_suspend() noexcept
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
		};

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
		Awaiter GetAwaiter()
		{
			return Awaiter{ myHandle.promise().get_future().share() };
		}

		[[nodiscard]]
		T Result() const
		{
			return myHandle.promise().get_future().get();
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
	};

	template<typename T>
	Task<T>::awaiter operator co_await(Task<T>& task)
	{
		return task.GetAwaiter();
	}

	template<typename T>
	Task<T>::awaiter operator co_await(Task<T>&& task)
	{
		return task.GetAwaiter();
	}
}
