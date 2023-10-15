export module Net.Task;
import Net.Constraints;
import Net.Coroutine.Suspender;
import Net.Coroutine.Promissory;
import Net.Coroutine.IPromise;
import <stdexcept>;
import <coroutine>;

export namespace net
{
	template<typename T = void, Suspender Init = std::suspend_always, Suspender Final = std::suspend_always>
	class Task;

	template<Suspender Init, Suspender Final>
	class Task<void, Init, Final> final
	{
	public:
		struct promise_type : public IPromise<Init, Final>
		{
			[[nodiscard]]
			Task<void, Init, Final> get_return_object() noexcept
			{
				return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
			}

			static constexpr void return_void() noexcept
			{}
		};
		static_assert(not Promissory<promise_type>);

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

		[[nodiscard]]
		constexpr bool operator==(const Task&) const noexcept = default;

	private:
		handle_type myHandle;
	};

	template<notvoids T, Suspender Init, Suspender Final>
	class [[nodiscard]] Task<T, Init, Final> final
	{
	public:
		struct promise_type : public IPromise<Init, Final>
		{
			[[nodiscard]]
			Task<T, Init, Final> get_return_object() noexcept
			{
				return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
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

		using handle_type = std::coroutine_handle<promise_type>;

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
		constexpr bool operator==(const Task&) const noexcept = default;

	private:
		handle_type myHandle;
		std::runtime_error reservedError;
	};
}
