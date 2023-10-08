export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Promissory;
import Net.Coroutine.Awaitable;
import <type_traits>;
import <coroutine>;

export namespace net
{
	template<Promissory Promise>
	class [[nodiscard]] Coroutine
	{
	public:
		using promise_type = Promise;
		using handle_type = std::coroutine_handle<Promise>;

		constexpr Coroutine() noexcept = default;

		constexpr Coroutine(const handle_type& handle)
			noexcept(nothrow_copy_constructibles<handle_type>) requires(copyable<handle_type>)
			: coHandle(handle)
		{}

		constexpr Coroutine(handle_type&& handle)
			noexcept(nothrow_move_constructibles<handle_type>) requires(movable<handle_type>)
			: coHandle(std::move(handle))
		{}

		virtual ~Coroutine() noexcept(noexcept(coHandle.destroy()))
		{
			coHandle.destroy();
		}

		[[nodiscard]]
		inline bool IsDone() const noexcept
		{
			return coHandle.done();
		}

		inline void Resume() const noexcept(noexcept(coHandle.resume()))
		{
			if (!coHandle.done())
			{
				coHandle.resume();
			}
		}

		inline void operator()() const noexcept
		{
			Resume();
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	protected:
		handle_type coHandle;
	};
}
