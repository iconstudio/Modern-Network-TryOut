export module Net.Task;
import Net.Constraints;
import Net.Coroutine;
import Net.Coroutine.Suspender;
import Net.Coroutine.ValuePromise;

export namespace net
{
	template<typename T, Suspender Init = std::suspend_always, Suspender Final = std::suspend_always>
	class Task;

	template<Suspender Init, Suspender Final>
	class Task<void, Init, Final> : public Coroutine<ValuePromise<void, Init, Final>>
	{
	public:
		using promise_type = ValuePromise<void, Init, Final>;
		using handle_type = std::coroutine_handle<promise_type>;
		using super = Coroutine<ValuePromise<void, Init, Final>>;

		constexpr Task() noexcept = default;

		constexpr Task(const handle_type& handle)
			noexcept(nothrow_copy_constructibles<handle_type>) requires(copyable<handle_type>)
			: super(handle)
		{}

		constexpr Task(handle_type&& handle)
			noexcept(nothrow_move_constructibles<handle_type>) requires(movable<handle_type>)
			: super(std::move(handle))
		{}

		~Task() noexcept = default;
	};

	template<typename T, Suspender Init, Suspender Final>
	class [[nodiscard]] Task : public Coroutine<ValuePromise<T, Init, Final>>
	{
	public:
		using promise_type = ValuePromise<T, Init, Final>;
		using handle_type = std::coroutine_handle<promise_type>;
		using super = Coroutine<ValuePromise<T, Init, Final>>;

		constexpr Task() noexcept = default;

		constexpr Task(const handle_type& handle)
			noexcept(nothrow_copy_constructibles<handle_type>) requires(copyable<handle_type>)
			: super(handle)
		{}

		constexpr Task(handle_type&& handle)
			noexcept(nothrow_move_constructibles<handle_type>) requires(movable<handle_type>)
			: super(std::move(handle))
		{}

		~Task() noexcept = default;
	};
}
