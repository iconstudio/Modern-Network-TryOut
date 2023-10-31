module;
#include <type_traits>

export module Net.NativeHandle;
import <format>;
import Net.Property;

export namespace net
{
	class [[nodiscard]] NativeHandle final
	{
	public:
		constexpr NativeHandle() noexcept = default;
		constexpr ~NativeHandle() noexcept = default;

		[[nodiscard]]
		constexpr const void* GetPointer() const& noexcept
		{
			return nativePointer;
		}

		[[nodiscard]]
		constexpr void*&& GetPointer() && noexcept
		{
			return static_cast<void*&&>(nativePointer);
		}

		[[nodiscard]]
		static constexpr NativeHandle Create(void* const& handle) noexcept
		{
			return NativeHandle{ handle };
		}

		constexpr NativeHandle& operator=(std::nullptr_t) noexcept
		{
			nativePointer = nullptr;
			return *this;
		}

		constexpr NativeHandle(NativeHandle&&) noexcept = default;
		constexpr NativeHandle& operator=(NativeHandle&&) noexcept = default;
		constexpr bool operator==(const NativeHandle&) const noexcept = default;

	private:
		constexpr NativeHandle(void* const& handle) noexcept
			: nativePointer(handle)
		{}

		NativeHandle(const NativeHandle&) = delete;
		NativeHandle& operator=(const NativeHandle&) = delete;

		Property<void*> nativePointer;
	};
}

export template<> struct std::formatter<net::NativeHandle>
{
	constexpr std::format_parse_context::iterator parse(std::format_parse_context& context) const
	{
		return context.begin();
	}

	std::format_context::iterator format(const net::NativeHandle& handle, std::format_context& context) const noexcept;
};

export namespace std
{
	[[nodiscard]]
	std::string to_string(const net::NativeHandle& handle) noexcept
	{
		return std::format("{}", handle);
	}
}
