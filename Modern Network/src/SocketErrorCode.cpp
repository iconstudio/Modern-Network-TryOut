module;
#include <WinSock2.h>
module Net.Socket.ErrorCode;

net::SocketErrorCodes
net::AcquireSocketError()
noexcept
{
	return static_cast<SocketErrorCodes>(WSAGetLastError());
}

net::SendingErrorCodes
net::AcquireSendingError()
noexcept
{
	return static_cast<net::SendingErrorCodes>(WSAGetLastError());
}

net::ReceivingErrorCodes
net::AcquireReceivingError()
noexcept
{
	return static_cast<net::ReceivingErrorCodes>(WSAGetLastError());
}


net::SocketOptionErrorCodes
net::AcquireSocketOptionsError()
noexcept
{
	return static_cast<net::SocketOptionErrorCodes>(WSAGetLastError());
}

net::SocketOpenErrorCodes
net::AcquireListeningError()
noexcept
{
	return static_cast<net::SocketOpenErrorCodes>(WSAGetLastError());
}

net::SocketClosingErrorCodes
net::AcquireClosingError()
noexcept
{
	return static_cast<net::SocketClosingErrorCodes>(WSAGetLastError());
}
