export module Net.Task;
import :Promise;
export import <stdexcept>;
export import <coroutine>;

export namespace net
{
	template<typename T>
	class [[nodiscard]] Task final
	{
	public:
		using promise_type = TaskPromise<T>;
		using handle_type = std::coroutine_handle<promise_type>;

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
		const static inline std::runtime_error reservedError{ "Cannot acquire a vale from the null promise" };

		handle_type myHandle;
	};
}
