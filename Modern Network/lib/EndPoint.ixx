export module Net.EndPoint;
import Net.IpAddress;
import <cstdint>;
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
		constexpr std::string_view GetAddress() const noexcept
		{
			return myAddress.GetAddress();
		}
		[[nodiscard]]
		constexpr std::uint16_t GetPort() const noexcept
		{
			return myPort;
		}
		[[nodiscard]]
		constexpr IpAddressFamily GetAddressFamily() const noexcept
		{
			return myAddress.GetFamily();
		}

	private:
		IpAddress myAddress;
		std::uint16_t myPort;
	};
}
