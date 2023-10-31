module Net.IpAddress;
import <utility>;
import :IpAddressFamily;

std::format_parse_context::iterator
std::formatter<net::IpAddressFamily>::parse(std::format_parse_context& context)
const noexcept
{
	return context.begin();
}

std::format_context::iterator
std::formatter<net::IpAddressFamily>::format(const net::IpAddressFamily& fm, std::format_context& context)
const noexcept
{
	switch (fm)
	{
		case net::IpAddressFamily::IPv4:
		{
			return std::format_to(context.out(), "IPv4 Family");
		}

		case net::IpAddressFamily::IPv6:
		{
			return std::format_to(context.out(), "IPv6 Family");
		}
	}

	return std::format_to(context.out(), "Unknown IP Address Family");
}
