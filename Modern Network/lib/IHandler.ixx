export module Net.IHandler;
import Net.Constraints;

export namespace net
{
	template<typename H>
	class [[nodiscard]] IHandler
	{
	public:
		constexpr IHandler() noexcept(nothrow_default_constructibles<H>) = default;
		virtual constexpr ~IHandler() noexcept(nothrow_destructibles<H>) = default;

		constexpr IHandler(const H& handle) noexcept(nothrow_copy_constructibles<H>)requires copy_constructible<H>
			: myHandle(handle)
		{}

		constexpr IHandler(H&& handle) noexcept(nothrow_move_constructibles<H>) requires move_constructible<H>
			: myHandle(handle)
		{}

		[[nodiscard]]
		constexpr const H& GetHandle() const noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr const volatile H& GetHandle() const volatile noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr bool operator==(const IHandler&) const noexcept = default;

	protected:
		H myHandle;
	};
}
