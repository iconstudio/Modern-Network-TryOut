module;
#define _RESUMABLE_FUNCTIONS_SUPPORTED
export module Net.Task;
import :Promise;
import Net.Constraints;
import Net.Coroutine.Suspender;
import Net.Coroutine.Promissory;
import Net.Coroutine.IPromise;
import <exception>;
import <stdexcept>;
import <coroutine>;
import <future>;

namespace net
{
	export template<typename T = void> class Task;

	export template<typename T> class [[nodiscard]] Task final
	{
	public:
		using promise_type = TaskPromise<T>;
		using handle_type = std::coroutine_handle<promise_type>;

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
		std::runtime_error reservedError{ "Cannot acquire a vale from the null promise" };
	};
}
