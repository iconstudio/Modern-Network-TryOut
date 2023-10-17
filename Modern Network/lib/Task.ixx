module;
#define _RESUMABLE_FUNCTIONS_SUPPORTED
export module Net.Task;
import Net.Constraints;
import Net.Coroutine.Suspender;
import Net.Coroutine.Promissory;
import Net.Coroutine.IPromise;
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
			static constexpr std::suspend_never final_suspend() noexcept
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
				myValue = std::forward<U>(value);
			}

			T myValue;
		};
		static_assert(not Promissory<promise_type>);

		constexpr Task(const handle_type& handle) noexcept
			: myHandle(handle), reservedError("Cannot acquire a vale from the null promise")
		{}
		constexpr Task(handle_type&& handle) noexcept
			: myHandle(std::move(handle)), reservedError("Cannot acquire a vale from the null promise")
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
		T& Current()
		{
			if (myHandle)
			{
				myHandle.resume();

				promise_type& promise = myHandle.promise();
				return promise.myValue;
			}
			else
			{
				throw reservedError;
			}
		}
		[[nodiscard]]
		const T& Current() const
		{
			if (myHandle)
			{
				myHandle.resume();

				promise_type& promise = myHandle.promise();
				return promise.myValue;
			}
			else
			{
				throw reservedError;
			}
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
		handle_type myHandle;
		std::runtime_error reservedError;
	};

	template <notvoids T, typename Init, typename Final>
	auto operator co_await(Task<T>& task) noexcept
		requires(!std::is_reference_v<T>)
	{
		struct awaiter
		{
			bool await_ready() const noexcept
			{
				return this->wait_for(0s) != std::future_status::timeout;
			}

			void await_suspend(std::coroutine_handle<> handle) const
			{
				std::thread([this, &handle] {
					this->wait();
					handle();
				}).detach();
			}

			T await_resume() { return this->get(); }
		};

		return awaiter{ std::move(task) };
	}
}
