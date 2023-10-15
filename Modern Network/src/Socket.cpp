module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

module Net.Socket;
import <type_traits>;

using namespace net;

const Socket::EmptySocketType Socket::EmptySocket = {};

Socket::Socket()
noexcept
	: Socket(EmptySocket)
{}

Socket::Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept
	: Handler(sock)
	, myProtocol(protocol), myFamily(family)
	, IsAddressReusable(this, false, SetAddressReusable)
{}

Socket::Socket(EmptySocketType)
noexcept
	: Socket(INVALID_SOCKET, InternetProtocols::Unknown, IpAddressFamily::Unknown)
{}

Socket&
Socket::operator=(EmptySocketType)
noexcept
{
	Close();

	return *this = Socket{ EmptySocket };
}

Socket::~Socket()
noexcept
{}

SocketListeningResult
Socket::Open()
const noexcept
{
	const int open = ::listen(myHandle, SOMAXCONN);
	if (SOCKET_ERROR == open)
	{
		return unexpected(AcquireListeningError());
	}

	return 0;
}

bool
Socket::Close()
noexcept
{
	if (IsAvailable())
	{
		return 0 == ::closesocket(myHandle);
	}
	else
	{
		return false;
	}
}

bool
Socket::Close(SocketClosingErrorCodes& error_code)
noexcept
{
	if (IsAvailable())
	{
		if (0 == ::closesocket(myHandle))
		{
			return true;
		}
		else
		{
			error_code = AcquireClosingError();
			return false;
		}
	}
	else
	{
		error_code = SocketClosingErrorCodes::NotASocket;
		return false;
	}
}

Socket
Socket::Create(const InternetProtocols& protocol, const IpAddressFamily& family)
noexcept
{
	NativeSocket result;
	switch (protocol)
	{
		case InternetProtocols::TCP:
		{
			result = WSASocket(static_cast<int>(family), SOCK_STREAM, IPPROTO::IPPROTO_TCP, nullptr, 0, WSA_FLAG_REGISTERED_IO);
		}
		break;

		case InternetProtocols::UDP:
		{
			result = WSASocket(static_cast<int>(family), SOCK_DGRAM, IPPROTO::IPPROTO_UDP, nullptr, 0, WSA_FLAG_REGISTERED_IO);
		}
		break;

		case InternetProtocols::Unknown:
		{
			return EmptySocket;
		}
	}

	return Socket(result, protocol, family);
}

Socket
Socket::Create(const InternetProtocols& protocol, const IpAddressFamily& family, SocketErrorCodes& error_code)
noexcept
{
	if (Socket result = Create(protocol, family); result.IsAvailable())
	{
		return result;
	}
	else
	{
		error_code = AcquireSocketError();
		return EmptySocket;
	}
}

bool
Socket::TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out)
noexcept
{
	if (Socket result = Create(protocol, family); result.IsAvailable())
	{
		out = AttentSocket
		{
			.Socket = std::move(result)
		};

		return true;
	}
	else
	{
		return false;
	}
}

bool
Socket::TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out, SocketErrorCodes& error_code)
noexcept
{
	if (Socket result = Create(protocol, family); result.IsAvailable())
	{
		out = AttentSocket
		{
			.Socket = std::move(result)
		};

		return true;
	}
	else
	{
		error_code = AcquireSocketError();
		return false;
	}
}

Socket::FactoryResult
Socket::TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family)
noexcept
{
	if (Socket result = Create(protocol, family); result.IsAvailable())
	{
		return std::move(result);
	}
	else
	{
		return unexpected(AcquireSocketError());
	}
}

void
Socket::SetAddressReusable(Socket& target, bool& flag)
noexcept
{}
