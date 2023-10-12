module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
module Net.IpAddress;

net::SerializedIpAddress
net::IpAddress::Serialize()
noexcept
{
	SOCKADDR_STORAGE sockaddr{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(sockaddr);

	::inet_pton((int)addressFamily, addressBuffer.get(), sockaddr_ptr);

	SerializedIpAddress result;
	const char* it = reinterpret_cast<const char*>(sockaddr_ptr);
	char* dest = reinterpret_cast<char*>(std::addressof(result));

	std::copy(it, it + sizeof(sockaddr), dest);

	return result;
}

bool
net::IpAddress::TrySerialize(net::SerializedIpAddress& result)
const noexcept
{
	SOCKADDR_STORAGE sockaddr{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(sockaddr);

	if (1 != ::inet_pton((int)addressFamily, addressBuffer.get(), sockaddr_ptr))
	{
		return false;
	}
	else
	{
		SerializedIpAddress result;
		const char* it = reinterpret_cast<const char*>(sockaddr_ptr);
		char* dest = reinterpret_cast<char*>(std::addressof(result));

		std::copy(it, it + sizeof(sockaddr), dest);

		return true;
	}
}
