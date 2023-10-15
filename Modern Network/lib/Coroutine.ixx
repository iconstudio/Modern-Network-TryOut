export module Net.Coroutine;
import Net.Coroutine.Promissory;
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

		constexpr Coroutine(const handle_type& handle) noexcept
			: myHandle(handle)
		{}

		constexpr Coroutine(handle_type&& handle) noexcept
			: myHandle(std::move(handle))
		{}

		virtual ~Coroutine() noexcept(noexcept(myHandle.destroy()))
		{
			if (myHandle)
			{
				myHandle.destroy();
			}
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	private:
		std::coroutine_handle<promise_type> myHandle;
	};
}
