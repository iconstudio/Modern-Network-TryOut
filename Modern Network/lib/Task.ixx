module;
#include <stdexcept>
#include <utility>
#include <thread>
#include <future>
#include <chrono>

export module Net.Task;
export import <coroutine>;

export namespace net
{
	template<typename T = void>
	class Task;

	template<typename T>
	class [[nodiscard]] Task final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;
		struct Awaiter;
		struct Finalizer;

		struct promise_type
		{
			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				return Task(handle_type::from_promise(*this), myHandle.get_future());
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
			std::coroutine_handle<void> previousFrame;
		};

		constexpr Task(const handle_type& handle, std::future<T>&& value_handle) noexcept
			: myHandle(handle), ownedValueHandle(std::move(value_handle))
		{}

		constexpr Task(handle_type&& handle, std::future<T>&& value_handle) noexcept
			: myHandle(std::move(handle)), ownedValueHandle(std::move(value_handle))
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

		T operator()()
		{
			myHandle();
			return ownedValueHandle.get();
		}

		Awaiter operator co_await()
		{
			return Awaiter
			{
				.coHandle = myHandle,
				.valueHandle = ownedValueHandle.share()
			};
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		struct Awaiter
		{
			bool await_ready() const noexcept
			{
				return isRetrieved and not valueHandle.valid();
			}

			std::coroutine_handle<void> await_suspend(std::coroutine_handle<void> previous_handle)
			{
				coHandle.promise().previousFrame = previous_handle;

				if (previous_handle and valueHandle.valid())
				{
					std::thread([this, previous_handle] {
						valueHandle.wait();
						isRetrieved = true;
						previous_handle();
					}).detach();

					return coHandle;
				}
				else
				{
					isRetrieved = true;

					return previous_handle;
				}
			}

			T await_resume() const
			{
				return valueHandle.get();
			}

			handle_type coHandle;
			std::shared_future<T> valueHandle;
			volatile bool isRetrieved;
		};

		struct Finalizer
		{
			static constexpr bool await_ready() noexcept { return false; }
			static constexpr void await_resume() noexcept {}

			std::coroutine_handle<void> await_suspend(handle_type current_frame) noexcept
			{
				// final_awaiter::await_suspend is called when the execution of the
				// current coroutine (referred to by 'current_frame') is about to finish.
				// If the current coroutine was resumed by another coroutine via
				// co_await get_task(), a handle to that coroutine has been stored
				// as current_frame.promise().previous. In that case, return the handle to resume
				// the previous coroutine.
				// Otherwise, return noop_coroutine(), whose resumption does nothing.

				if (std::coroutine_handle<void> previous = current_frame.promise().previousFrame; previous)
				{
					return previous;
				}
				else
				{
					return std::noop_coroutine();
				}
			}
		};

		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
		std::future<T> ownedValueHandle;
	};
}

