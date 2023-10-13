export module Net.IpAddress.IPv4;
export import Net.IpAddress;
import Net.Property;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IPv4Address final
	{
	public:
		static ReadonlyProperty<IpAddress> Loopback;

		[[nodiscard]]
		static IpAddress Create(std::string_view address) noexcept
		{
			return IpAddress{ IpAddressFamily::IPv4, address };
		}

	private:
		constexpr IPv4Address() noexcept = default;
		constexpr ~IPv4Address() noexcept = default;
	};

	ReadonlyProperty<IpAddress> IPv4Address::Loopback = IPv4Address::Create("127.0.0.1");
}
