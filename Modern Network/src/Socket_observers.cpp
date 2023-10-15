module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
module Net.Socket;

bool
net::Socket::IsAvailable()
const noexcept
{
	return myHandle != INVALID_SOCKET;
}
