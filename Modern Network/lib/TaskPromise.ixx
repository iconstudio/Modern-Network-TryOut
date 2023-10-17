export module Net.Task:Promise;

export namespace net
{
	template<typename T> class Task;

	template<typename T>
	struct TaskEngine
	{
		bool await_ready() const noexcept;

		void await_suspend(handle_type handle) const;

		T await_resume();

		std::future<T> valueHandle;
	};

	template<typename T>
	struct TaskPromise
	{
		[[nodiscard]]
		Task<T> get_return_object() noexcept
		{
			return Task{ handle_type::from_promise(*this) };
		}

		template<typename U>
		void return_value(U&& value) noexcept(not same_as<T, void> ? nothrow_assignable<T, U&&> : true)
		{
			if constexpr (same_as<T, void>)
			{
				myHandle.set_value();
			}
			else
			{
				myHandle.set_value(std::forward<U>(value));
				static_assert(not convertible_to<T, U>);
			}
		}

		TaskEngine initial_suspend() noexcept
		{
			return TaskEngine{ myHandle.get_future() };
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
	};
}
