export module Net.Task;
import <stdexcept>;
import <utility>;
import <thread>;
import <latch>;
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
				return Task<T>(handle_type::from_promise(*this));
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

		void Start() const
		{
			if (myHandle)
			{
				myHandle.resume();
			}
		}

		void StartAsync() const
		{
			if (myHandle)
			{
				std::thread{
					[h = myHandle] {
					h.resume();
				} }.detach();
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
				static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
				{}

				T await_resume()
				{
					coHandle();
					return std::move(coHandle.promise().myValue);
				}

				handle_type coHandle;
			};

			return awaiter{ .coHandle = myHandle };
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
	};

	template<>
	class [[nodiscard]] Task<void> final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			promise_type() noexcept = default;
			~promise_type() noexcept = default;

			[[nodiscard]]
			Task<void> get_return_object() noexcept
			{
				return Task<void>(handle_type::from_promise(*this));
			}

			static constexpr void return_void() noexcept {}

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

		void Start() const
		{
			if (myHandle)
			{
				myHandle.resume();
			}
		}

		void StartAsync() const
		{
			if (myHandle)
			{
				std::thread{
					[h = myHandle] {
					h.resume();
				} }.detach();
			}
		}

		void operator()()
		{
			if (myHandle)
			{
				myHandle.resume();
			}
		}

		auto operator co_await()
		{
			struct awaiter
			{
				static constexpr bool await_ready() noexcept { return true; }
				static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
				{}

				void await_resume()
				{
					coHandle();
				}

				handle_type coHandle;
			};

			return awaiter{ .coHandle = myHandle };
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
	};
}

