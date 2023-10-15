module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
module Net.Socket;

net::SocketReceivingResult
net::Socket::Receive(std::span<std::byte> memory)
const noexcept
{
	return SocketReceivingResult();
}

net::SocketReceivingResult
net::Socket::Receive(const std::byte* const& memory, size_t size)
const noexcept
{
	return SocketReceivingResult();
}
