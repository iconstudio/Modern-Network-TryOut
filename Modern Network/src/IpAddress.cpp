module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
module Net.IpAddress;

net::SerializedIpAddress
net::IpAddress::Serialize()
const noexcept
{
	SerializedIpAddress result{};

	::inet_pton((int)addressFamily, addressBuffer.get(),  std::addressof(result));

	return result;
}

bool
net::IpAddress::TrySerialize(net::SerializedIpAddress& out)
const noexcept
{
	SerializedIpAddress result{};

	if (1 != ::inet_pton((int)addressFamily, addressBuffer.get(), std::addressof(result)))
	{
		return false;
	}
	else
	{
		out = std::move(result);
		return true;
	}
}

bool
net::IpAddress::TrySerialize(void* const& out)
const noexcept
{
	if (1 != ::inet_pton((int)addressFamily, addressBuffer.get(), out))
	{
		return false;
	}
	else
	{
		return true;
	}
}
