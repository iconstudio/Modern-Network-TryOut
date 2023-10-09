export module Net.IpAddress.IPv6;
import Net.IpAddress;
import <string_view>;

export namespace net
{
	class [[nodiscard]] IPv6Address final : public IpAddress
	{
	public:
		static const IPv6Address Loopback;

		constexpr IPv6Address() noexcept = default;
		constexpr ~IPv6Address() noexcept = default;

		constexpr IPv6Address(std::string_view address) noexcept
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
			return IpAddressType::IPv6;
		}

		[[nodiscard]]
		static constexpr IPv6Address Create(std::string_view address) noexcept
		{
			return IPv6Address{ address };
		}

		constexpr IPv6Address& operator=(std::string_view address) noexcept
		{
			return *this = IPv6Address{ address };
		}

		[[nodiscard]]
		constexpr bool operator==(const IPv6Address& other) const noexcept = default;

		constexpr IPv6Address(const IPv6Address&) noexcept = default;
		constexpr IPv6Address& operator=(const IPv6Address&) noexcept = default;
		constexpr IPv6Address(IPv6Address&&) noexcept = default;
		constexpr IPv6Address& operator=(IPv6Address&&) noexcept = default;

	private:
		char ipAddress[48];
	};

	const IPv6Address IPv6Address::Loopback = IPv6Address{ ":::::1" };
}
