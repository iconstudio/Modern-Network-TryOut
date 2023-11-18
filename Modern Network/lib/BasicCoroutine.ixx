export module  Net.Coroutine:BasicCoroutine;
import Net.Handler;
export import <coroutine>;

export namespace net::coroutine
{
	using std::suspend_never;
	using std::suspend_always;
	using std::coroutine_handle;

	template<typename Co, typename Promise>
	class BasicCoroutine : public Handler<coroutine_handle<Promise>>
	{
	public:
		using promise_type = Promise;
		using handle_type = coroutine_handle<promise_type>;
		using super = Handler<handle_type>;

		using super::super;
		using super::GetHandle;

		virtual ~BasicCoroutine() noexcept(noexcept(GetHandle().destroy()))
		{
			if (GetHandle())
			{
				GetHandle().destroy();
			}
		}

		void Resume() const noexcept(noexcept(Co::Resume()))
		{
			Co::Resume();
		}

		void operator()() const noexcept(noexcept(Co::operator()()))
		{
			Co::operator()();
		}

		[[nodiscard]]
		bool IsAvailable() const noexcept
		{
			return GetHandle();
		}

		[[nodiscard]]
		bool IsDone() const noexcept
		{
			return GetHandle().done();
		}

		[[nodiscard]]
		constexpr bool operator==(const BasicCoroutine&) const noexcept = default;
	};
}
