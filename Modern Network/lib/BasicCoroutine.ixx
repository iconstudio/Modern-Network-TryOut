export module  Net.Coroutine:BasicCoroutine;
import Net.Coroutine.Promissory;
export import <coroutine>;

export namespace net::coroutine
{
	using std::suspend_never;
	using std::suspend_always;
	using std::coroutine_handle;

	template<Promissory Promise>
	class BasicCoroutine
	{
	public:
		using promise_type = Promise;
		using handle_type = std::coroutine_handle<promise_type>;

		constexpr BasicCoroutine(const handle_type& handle) noexcept
			: myHandle(handle)
		{}

		constexpr BasicCoroutine(handle_type&& handle) noexcept
			: myHandle(static_cast<handle_type&&>(handle))
		{}

		~BasicCoroutine() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		void Resume() const
		{
			if (myHandle)
			{
				myHandle.resume();
			}
		}

		void operator()() const
		{
			if (myHandle)
			{
				myHandle();
			}
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return myHandle.done();
		}

		[[nodiscard]]
		constexpr bool operator==(const BasicCoroutine&) const noexcept = default;

	private:
		handle_type myHandle;
	};
}
