export module Net.IHandler;
import Net.Constraints;

export namespace net
{
	template<typename H>
	class [[nodiscard]] IHandler
	{
	public:
		constexpr IHandler()
			noexcept(nothrow_default_constructibles<H>) requires default_initializables<H> = default;
		virtual constexpr ~IHandler()
			noexcept(nothrow_destructibles<H>) = default;

		constexpr IHandler(const H& handle) noexcept(nothrow_copy_constructibles<H>) requires copy_constructible<H>
			: myHandle(handle)
		{}

		constexpr IHandler(H&& handle) noexcept(nothrow_move_constructibles<H>) requires move_constructible<H>
			: myHandle(handle)
		{}

		[[nodiscard]]
		constexpr const H& GetHandle() const& noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr const volatile H& GetHandle() const volatile& noexcept
		{
			return myHandle;
		}

		[[nodiscard]]
		constexpr H&& GetHandle() && noexcept
		{
			return static_cast<H&&>(myHandle);
		}

		[[nodiscard]]
		constexpr volatile H&& GetHandle() volatile&& noexcept
		{
			return static_cast<volatile H&&>(myHandle);
		}

		[[nodiscard]]
		constexpr bool operator==(const IHandler&) const noexcept = default;

		constexpr IHandler(const IHandler&)
			noexcept(nothrow_copy_constructibles<H>) requires copy_constructible<H> = default;
		constexpr IHandler(IHandler&&)
			noexcept(nothrow_move_constructibles<H>) requires move_constructible<H> = default;
		constexpr IHandler& operator=(const IHandler&)
			noexcept(nothrow_copy_assignables<H>) requires copy_assignables<H> = default;
		constexpr IHandler& operator=(IHandler&&)
			noexcept(nothrow_move_assignables<H>) requires move_assignables<H> = default;

	protected:
		H myHandle;
	};
}
