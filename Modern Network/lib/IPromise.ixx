export module Net.Coroutine.IPromise;
import Net.Constraints;
import Net.Coroutine.Suspender;
import Net.Coroutine.Promissory;
import Net.Coroutine.Awaitable;
import <coroutine>;

export namespace net
{
	template<typename Promise
		, Suspender Init = std::suspend_always
		, Suspender Final = std::suspend_always>
	class [[nodiscard]] IPromise
	{
	public:
		using type = IPromise<Promise, Init, Final>;
		using handle_type = std::coroutine_handle<type>;

		constexpr IPromise() noexcept = default;
		constexpr ~IPromise() noexcept = default;

		auto get_return_object() noexcept
		{
			return handle_type::from_address(this);
		}

		static constexpr std::decay_t<Init> initial_suspend() noexcept
		{
			return {};
		}

		static constexpr std::decay_t<Final> final_suspend() noexcept
		{
			return {};
		}

		[[noreturn]]
		static constexpr void return_void() noexcept {}

		[[noreturn]]
		static void unhandled_exception()
		{
			throw;
		}

	protected:
		[[nodiscard]]
		constexpr Promise* _Cast() noexcept
		{
			return static_cast<Promise*>(this);
		}

		[[nodiscard]]
		constexpr const Promise* _Cast() const noexcept
		{
			return static_cast<const Promise*>(this);
		}
	};
}
