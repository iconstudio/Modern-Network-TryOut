module;
#include <stdexcept>
#include <utility>
#include <thread>

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

		struct promise_type
		{
			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				return Task(handle_type::from_promise(*this));
			}

			template<typename U>
			void return_value(U&& value)
			{
				myValue = std::forward<U>(value);
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
				throw;
			}

			T myValue;
#if _DEBUG
			std::coroutine_handle<void> previousFrame;
#endif // _DEBUG
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

		T operator()()
		{
			if (myHandle)
			{
				myHandle.resume();
			}

			return std::move(myHandle.promise().myValue);
		}

		Awaiter operator co_await()
		{
			return Awaiter
			{
				.coHandle = myHandle,
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
			static constexpr bool await_ready() noexcept
			{
				return false;
			}

			std::coroutine_handle<void> await_suspend(std::coroutine_handle<void> previous_handle)
			{
#if _DEBUG
				coHandle.promise().previousFrame = previous_handle;
#endif // _DEBUG

				return coHandle;
			}

			T& await_resume()&
			{
				return coHandle.promise().myValue;
			}

			const T& await_resume() const&
			{
				return coHandle.promise().myValue;
			}

			T&& await_resume()&&
			{
				return std::move(coHandle.promise().myValue);
			}

			const T&& await_resume() const&&
			{
				return std::move(coHandle.promise().myValue);
			}

			handle_type coHandle;
		};

		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
	};
}

