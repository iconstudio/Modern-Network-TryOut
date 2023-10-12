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
	SerializedIpAddress* sockaddr_ptr = std::addressof(result);

	::inet_pton((int)addressFamily, addressBuffer.get(), sockaddr_ptr);

	return result;
}

bool
net::IpAddress::TrySerialize(net::SerializedIpAddress& out)
const noexcept
{
	SerializedIpAddress* sockaddr_ptr = std::addressof(out);

	if (1 != ::inet_pton((int)addressFamily, addressBuffer.get(), out))
	{
		return false;
	}
	else
	{
		return true;
	}
}