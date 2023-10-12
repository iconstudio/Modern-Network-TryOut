export module Net.EndPoint;
import Net.IpAddress;
import <cstdint>;
import <type_traits>;
import <string_view>;

export namespace net
{
	struct [[nodiscard]] EndPoint final
	{
	public:
		constexpr EndPoint(const IpAddress& ip, const std::uint16_t& port) noexcept
			: myAddress(ip), myPort(port)
		{}
		constexpr EndPoint(IpAddress&& ip, const std::uint16_t& port) noexcept
			: myAddress(static_cast<IpAddress&&>(ip)), myPort(port)
		{}
		constexpr ~EndPoint() noexcept = default;

		[[nodiscard]]
		constexpr IpAddressFamily GetAddressFamily() const& noexcept
		{
			return myAddress.GetFamily();
		}
		[[nodiscard]]
		constexpr const IpAddress& GetIpAddress() const& noexcept
		{
			return myAddress;
		}
		[[nodiscard]]
		constexpr std::string_view GetAddress() const& noexcept
		{
			return myAddress.GetAddress();
		}
		[[nodiscard]]
		constexpr std::uint16_t GetPort() const& noexcept
		{
			return myPort;
		}
		[[nodiscard]]
		constexpr IpAddressFamily&& GetAddressFamily() && noexcept
		{
			return std::move(myAddress).GetFamily();
		}
		[[nodiscard]]
		constexpr IpAddress&& GetIpAddress() && noexcept
		{
			return std::move(myAddress);
		}
		[[nodiscard]]
		constexpr decltype(auto) GetAddress() && noexcept
		{
			return std::move(myAddress).GetAddress();
		}
		[[nodiscard]]
		constexpr std::uint16_t GetPort() && noexcept
		{
			return std::move(myPort);
		}

	private:
		IpAddress myAddress;
		std::uint16_t myPort;
	};
}
