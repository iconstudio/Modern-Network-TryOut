export module Net.IpAddress:IpAddressType;

export namespace net
{
	enum class [[nodiscard]] IpAddressType
	{
		Unknown, IPv4, IPv6
	};
}
