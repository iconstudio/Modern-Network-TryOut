export module Net.IpAddress.IPv4;
export import Net.IpAddress;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IPv4Address final : public IpAddress
	{
	public:
		static const IPv4Address Loopback;

		constexpr IPv4Address() noexcept = default;
		constexpr ~IPv4Address() noexcept = default;

		constexpr IPv4Address(std::string_view address) noexcept
			: ipAddress()
		{
			std::copy(address.begin(), address.end(), ipAddress);
		}

		[[nodiscard]]
		constexpr std::string_view GetAddress() const noexcept override
		{
			return ipAddress;
		}
		[[nodiscard]]
		constexpr IpAddressFamily GetType() const noexcept override
		{
			return IpAddressFamily::IPv4;
		}

		[[nodiscard]]
		static constexpr IPv4Address Create(std::string_view address) noexcept
		{
			return IPv4Address{ address };
		}

		constexpr IPv4Address& operator=(std::string_view address) noexcept
		{
			return *this = IPv4Address{ address };
		}

		[[nodiscard]]
		constexpr bool operator==(const IPv4Address& other) const noexcept = default;

		constexpr IPv4Address(const IPv4Address&) noexcept = default;
		constexpr IPv4Address& operator=(const IPv4Address&) noexcept = default;
		constexpr IPv4Address(IPv4Address&&) noexcept = default;
		constexpr IPv4Address& operator=(IPv4Address&&) noexcept = default;

	private:
		char ipAddress[16];
	};

	const IPv4Address IPv4Address::Loopback = IPv4Address{ "127.0.0.1" };
}
