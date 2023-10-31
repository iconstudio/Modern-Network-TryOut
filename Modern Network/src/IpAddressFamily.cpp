module Net.IpAddress;
import <utility>;
import :IpAddressFamily;

std::format_context::iterator
std::formatter<net::IpAddressFamily>::format(const net::IpAddressFamily& fm, std::format_context& context)
const noexcept
{
	return std::format_to(context.out(), "IP Address [{} / {}]", ip.GetFamily(), ip.GetAddress());
}
