export module Net.IpAddress.IPv6;
export import Net.IpAddress;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IPv6Address final
	{
	public:
		static const IPv6Address Loopback;

		[[nodiscard]]
		static constexpr IpAddress Create(std::string_view address) noexcept
		{
			return IpAddress{ IpAddressFamily::IPv6, address };
		}

	private:
		constexpr IPv6Address() noexcept = default;
		constexpr ~IPv6Address() noexcept = default;
	};

	const IPv6Address IPv6Address::Loopback = IPv6Address{ ":::::1" };
}
