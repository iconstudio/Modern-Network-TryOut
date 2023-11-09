module;
#include <stdexcept>
#include <future>
#include <thread>

export module Net.Task;
import Net.Constraints;
import Net.Coroutine;

export namespace net
{
	template<typename T = void>
	class Task;

	template<notvoids T>
	class Task<T> final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;
		using promise_handle_type = std::promise<T>;
		using future_type = std::future<T>;
		using public_future_type = std::shared_future<T>;

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
				myHandle.set_value(std::forward_like<T>(value));
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

			promise_handle_type myHandle;
		};

		Task(const handle_type& handle, future_type&& future) noexcept
			: myHandle(handle), valueHandle(std::move(future))
		{}

		Task(handle_type&& handle, future_type&& future) noexcept
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
			return false;
		}

		void await_suspend(std::coroutine_handle<void>)
		{
			std::thread([this] {
				valueHandle.wait();
				myValue = valueHandle.get();
				myHandle();
			}).detach();
		}

		T await_resume() const
		{
			return myValue;
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		void operator()() const
		{
			if (myHandle)
			{
				myHandle();
			}
		}

		[[nodiscard]]
		const T& Result() const
		{
			return myValue;
		}

		[[nodiscard]]
		constexpr bool operator==(const Task& other) const noexcept
		{
			return myHandle.address() == other.myHandle.address();
		}

	private:
		const static inline std::runtime_error reservedError{ "Cannot acquire a value from the null promise" };

		handle_type myHandle;
		future_type valueHandle;
		T myValue;
	};

	template<>
	class Task<void> final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;
		using promise_handle_type = std::promise<void>;
		using future_type = std::future<void>;
		using public_future_type = std::shared_future<void>;

		struct promise_type
		{
			[[nodiscard]]
			Task<void> get_return_object() noexcept;
			void return_void();

			static constexpr std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			static constexpr std::suspend_always final_suspend() noexcept
			{
				return {};
			}

			[[noreturn]]
			void unhandled_exception();

			promise_handle_type myHandle;
			future_type myValueHandle;
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
			return false;
		}

		void await_suspend(std::coroutine_handle<void>) const noexcept
		{
			std::thread([this] {
				valueHandle.wait();
				myHandle();
			}).detach();
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
			if (myHandle)
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
