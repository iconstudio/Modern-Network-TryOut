export module Net.Coroutine;
import Net.Constraints;
import Net.Coroutine.Promissory;
import Net.Coroutine.Awaitable;
import <type_traits>;
import <coroutine>;

export namespace net
{
	template<Promissory Promise>
	class [[nodiscard]] Coroutine : public std::coroutine_handle<Promise>
	{
	public:
		using promise_type = Promise;
		using super = std::coroutine_handle<Promise>;

		using super::super;

		virtual ~Coroutine() noexcept(noexcept(super::destroy()))
		{
			super::destroy();
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;
	};
}
