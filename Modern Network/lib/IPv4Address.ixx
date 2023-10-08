export module Net.IpAddress.IPv4;
import Net.IpAddress;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IPv4Address : public IpAddress
	{
	public:
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
		constexpr IpAddressType GetType() const noexcept override
		{
			return IpAddressType::IPv4;
		}

		[[nodiscard]]
		static constexpr IPv4Address Create(std::string_view address) noexcept
		{
			return IPv4Address{ address };
		}

		[[nodiscard]]
		constexpr bool operator==(const IPv4Address& other) const noexcept = default;

		constexpr IPv4Address(const IPv4Address&) noexcept = default;
		constexpr IPv4Address(IPv4Address&&) noexcept = default;

	private:
		char ipAddress[16];
	};
}
