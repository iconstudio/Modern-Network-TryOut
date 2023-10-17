export module Net.Task:Promise;
import Net.Constraints;
import <future>;
import <coroutine>;

export namespace net
{
	template<typename T> class Task;

	template<typename T>
	struct TaskEngine
	{
		bool await_ready() const noexcept;

		void await_suspend(std::coroutine_handle<> handle) const;

		T await_resume();

		std::future<T> valueHandle;
	};

	template<typename T>
	struct TaskPromise
	{
		[[nodiscard]]
		Task<T> get_return_object() noexcept;
		template<typename U>
		void return_value(U&& value);

		TaskEngine<T> initial_suspend() noexcept;
		static constexpr std::suspend_always final_suspend() noexcept
		{
			return {};
		}

		[[noreturn]]
		void unhandled_exception();

		std::promise<T> myHandle;
	};
}
