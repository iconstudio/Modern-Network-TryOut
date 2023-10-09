export module Net.EndPoint;
import Net.IpAddress;
import <string_view>;

export namespace net
{
	class [[nodiscard]] EndPoint final
	{
	public:
		[[nodiscard]]
		constexpr std::string_view GetAddress() const noexcept
		{
			return myAddress.GetAddress();
		}
		[[nodiscard]]
		constexpr unsigned short GetPort() const noexcept
		{
			return myPort;
		}
		[[nodiscard]]
		constexpr IpAddressType GetType() const noexcept
		{
			return myAddress.GetType();
		}
		[[nodiscard]]
		constexpr unsigned int GetAddressFamily() const noexcept
		{
			return addressFamily;
		}

	private:
		IpAddress&& myAddress;
		unsigned short&& myPort;
		unsigned int&& addressFamily;
	};
}
