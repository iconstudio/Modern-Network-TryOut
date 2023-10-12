export module Net.IpAddress;
export import :IpAddressFamily;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IpAddress
	{
	public:
		constexpr IpAddress() noexcept = default;
		virtual constexpr ~IpAddress() noexcept = default;

		[[nodiscard]]
		virtual constexpr std::string_view GetAddress() const noexcept = 0;
		[[nodiscard]]
		virtual constexpr IpAddressFamily GetType() const noexcept = 0;

		[[nodiscard]]
		constexpr bool operator==(const IpAddress& other) const noexcept = default;
	};
}
