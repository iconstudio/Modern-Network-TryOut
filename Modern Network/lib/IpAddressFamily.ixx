export module Net.IpAddress:IpAddressFamily;

export namespace net
{
	enum class [[nodiscard]] IpAddressFamily
	{
		Unknown, IPv4 = 2, IPv6 = 23
	};
}
