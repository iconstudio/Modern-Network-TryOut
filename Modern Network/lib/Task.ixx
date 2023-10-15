export module Net.Task;
import Net.Constraints;
export import Net.Coroutine;
export import Net.Coroutine.Suspender;
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
		using super = Coroutine<ValuePromise<void, Init, Final>>;

		using super::super;

		~Task() noexcept = default;
	};

	template<typename T, Suspender Init, Suspender Final>
	class [[nodiscard]] Task : public Coroutine<ValuePromise<T, Init, Final>>
	{
	public:
		using promise_type = ValuePromise<T, Init, Final>;
		using super = Coroutine<ValuePromise<T, Init, Final>>;

		using super::super;

		~Task() noexcept = default;
	};
}
