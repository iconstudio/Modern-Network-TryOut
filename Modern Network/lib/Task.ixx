export module Net.Task;
import <stdexcept>;
import <utility>;
import <thread>;
import <atomic>;
import <coroutine>;

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

		struct promise_type
		{
			promise_type() noexcept = default;
			~promise_type() noexcept = default;

			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				return Task<T>(handle_type::from_promise(*this), myEvent);
			}

			template<typename U>
			void return_value(U&& value)
			{
				myValue = std::forward<U>(value);
				myEvent.test_and_set();
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
			volatile std::atomic_flag myEvent;
#if _DEBUG
			std::coroutine_handle<void> previousFrame;
#endif // _DEBUG
		};

		constexpr Task(const handle_type& handle, volatile std::atomic_flag& event) noexcept
			: myHandle(handle), coEvent(event)
		{}

		constexpr Task(handle_type&& handle, volatile std::atomic_flag& event) noexcept
			: myHandle(std::move(handle)), coEvent(event)
		{}

		~Task() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		T operator()()
		{
			if (myHandle)
			{
				myHandle.resume();
			}

			return std::move(myHandle.promise().myValue);
		}

		auto operator co_await()
		{
			struct awaiter
			{
				static constexpr bool await_ready() noexcept { return true; }

				auto await_suspend(std::coroutine_handle<void> previous_handle)
				{
#if _DEBUG
					coHandle.promise().previousFrame = previous_handle;
#endif // _DEBUG

					std::thread{
						[this, previous_handle]() noexcept {
						while (not coEvent.test())
						{
						}
						previous_handle();
					} }.detach();

					return coHandle;
				}

				T await_resume()
				{
					coHandle();
					return std::move(coHandle.promise().myValue);
				}

				void Wait() const noexcept
				{
					while (not coEvent.test())
					{
					}
				}

				handle_type coHandle;
				volatile std::atomic_flag& coEvent;
			};

			return awaiter{ .coHandle = myHandle, .coEvent = coEvent };
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
		volatile std::atomic_flag& coEvent;
	};
}

