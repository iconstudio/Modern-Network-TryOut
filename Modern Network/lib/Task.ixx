module;
#include <future>
#include <coroutine>

export module Net.Task;
import <stdexcept>;
import <thread>;
import Net.Constraints;

namespace net
{
	template<typename Promise>
	struct Finalizer
	{
		static constexpr bool await_ready() noexcept
		{
			return false;
		}

		std::coroutine_handle<void> await_suspend(Promise::handle_type current) const noexcept
		{
			if (auto previous = current.promise().previousFrame; previous)
			{
				return previous;
			}
			else
			{
				return std::noop_coroutine();
			}
		}

		static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
		{}

		static constexpr void await_resume() noexcept
		{}
	};
}

export namespace net
{
	template<typename T>
	class Task final
	{
	public:
		struct promise_type;
		using handle_type = std::coroutine_handle<promise_type>;
		using promise_handle_type = std::promise<T>;
		using future_type = std::future<T>;
		using public_future_type = std::shared_future<T>;

		struct promise_type
		{
			using handle_type = Task<T>::handle_type;

			[[nodiscard]]
			Task<T> get_return_object() noexcept
			{
				return Task<T>(handle_type::from_promise(*this), myEventHandle, myValueHandle);
			}

			template<typename U>
			void return_value(U&& value)
			{
				myValueHandle = std::forward_like<T>(value);

				myEventHandle.notify_one();
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
			static void unhandled_exception()
			{
				throw;
			}

			std::atomic_flag myEventHandle;
			T myValueHandle;
		};

		Task(const handle_type& handle, std::atomic_flag& event_handle, T& value_ref) noexcept
			: myHandle(handle), myEvent(event_handle), myValue(value_ref)
		{}

		Task(handle_type&& handle, std::atomic_flag& event_handle, T& value_ref) noexcept
			: myHandle(std::move(handle)), myEvent(event_handle), myValue(value_ref)
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

		handle_type await_suspend(std::coroutine_handle<void> previous_frame)
		{
			std::thread([this] {
				myEvent.wait(true);
				myHandle();
			}).detach();

			return myHandle;
		}

		T& await_resume()&
		{
			return myValue;
		}

		const T& await_resume() const&
		{
			return myValue;
		}

		T&& await_resume()&&
		{
			return std::move(myValue);
		}

		const T&& await_resume() const&&
		{
			return std::move(myValue);
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		T& operator()()
		{
			if (myHandle)
			{
				myEvent.wait(true);
				myHandle();
			}

			return myValue;
		}

		[[nodiscard]]
		T& Result()
		{
			if (not myEvent.test())
			{
				throw reservedError;
			}

			return myValue;
		}

		[[nodiscard]]
		const T& Result() const
		{
			if (not myEvent.test())
			{
				throw reservedError;
			}

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
		std::atomic_flag& myEvent;
		T& myValue;
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
			using handle_type = Task<void>::handle_type;

			[[nodiscard]]
			Task<void> get_return_object() noexcept;
			void return_void();

			static constexpr std::suspend_always initial_suspend() noexcept
			{
				return {};
			}

			Finalizer<promise_type> final_suspend() noexcept
			{
				return {};
			}

			[[noreturn]]
			void unhandled_exception();

			promise_handle_type myHandle;
			future_type myValueHandle;
			std::coroutine_handle<void> previousFrame;
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

		void await_suspend(std::coroutine_handle<void> previous_frame) const noexcept
		{
			myHandle.promise().previousFrame = previous_frame;

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
