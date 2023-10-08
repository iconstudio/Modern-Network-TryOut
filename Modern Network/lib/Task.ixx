export module Net.Task;
import Net.Coroutine;
import Net.Coroutine.ValuePromise;

export namespace net
{
	template<typename T>
	class Task;

	template<>
	class Task<void> : public Coroutine<ValuePromise<void>>
	{
	public:
		constexpr Task() noexcept = default;
		~Task() noexcept = default;
	};

	template<typename T>
	class Task : public Coroutine<ValuePromise<T>>
	{
	public:
		constexpr Task() noexcept = default;
		~Task() noexcept = default;
	};
}
