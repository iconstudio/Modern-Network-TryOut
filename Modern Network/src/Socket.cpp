module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <type_traits>

module Net.Socket;
import <cstdint>;
import <expected>;
import <coroutine>;

using namespace net;

::SOCKADDR_STORAGE SerializeEndpoint(const net::EndPoint& endpoint) noexcept;
::SOCKADDR_STORAGE SerializeEndpoint(net::EndPoint&& endpoint) noexcept;

const Socket::EmptySocketType Socket::EmptySocket = {};

SocketSendingResult
Socket::Send(std::span<const std::byte> memory)
const noexcept
{
	WSABUF buffer
	{
		.len = static_cast<ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	DWORD bytes = 0;
	int result = WSASend(myHandle, std::addressof(buffer), 1, std::addressof(bytes), 0, nullptr, nullptr);
	if (0 == result)
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireSendingError(); error != SendingErrorCodes::PendedIoOperation)
		{
			return std::unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}

SocketSendingResult
Socket::Send(const std::byte* const& memory, size_t size)
const noexcept
{
	return SocketSendingResult();
}

SocketReceivingResult
Socket::Receive(std::span<std::byte> memory)
const noexcept
{
	return SocketReceivingResult();
}

SocketReceivingResult
Socket::Receive(const std::byte* const& memory, size_t size)
const noexcept
{
	return SocketReceivingResult();
}

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

SocketResult
Socket::Bind(const IpAddress& address, const std::uint16_t& port)
const noexcept
{
	return this->Bind(EndPoint(address, port));
}

SocketResult
Socket::Bind(IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	return this->Bind(EndPoint(std::move(address), port));
}

SocketResult
Socket::Bind(const EndPoint& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(endpoint);
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::bind(myHandle, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)))
	{
		return std::unexpected(AcquireSocketError());
	}

	return static_cast<unsigned int>(sizeof(sockaddr));
}

SocketResult
Socket::Bind(EndPoint&& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(std::move(endpoint));
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::bind(myHandle, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)))
	{
		return std::unexpected(AcquireSocketError());
	}

	return static_cast<unsigned int>(sizeof(sockaddr));
}

SocketListeningResult
Socket::Open()
const noexcept
{
	const int open = ::listen(myHandle, SOMAXCONN);
	if (SOCKET_ERROR == open)
	{
		return std::unexpected(AcquireListeningError());
	}

	return 0;
}

SocketResult
net::Socket::Connect(const IpAddress& address, const std::uint16_t& port)
const noexcept
{
	return this->Connect(EndPoint(address, port));
}

SocketResult
Socket::Connect(IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	return this->Connect(EndPoint(std::move(address), port));
}

SocketResult
Socket::Connect(const EndPoint& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(endpoint);
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::WSAConnect(myHandle
		, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)
		, nullptr, nullptr
		, nullptr, nullptr))
	{
		auto error_code = AcquireSocketError();
		if (error_code == SocketErrorCodes::NonBlockedOperation)
		{
			return 0;
		}
		else
		{
			return std::unexpected(error_code);
		}
	}

	return 1;
}

SocketResult
Socket::Connect(EndPoint&& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(std::move(endpoint));
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::WSAConnect(myHandle
		, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)
		, nullptr, nullptr
		, nullptr, nullptr))
	{
		auto error_code = AcquireSocketError();
		if (error_code == SocketErrorCodes::NonBlockedOperation)
		{
			return 0;
		}
		else
		{
			return std::unexpected(error_code);
		}
	}

	return 1;
}

import <future>;

Task<SocketResult>
Socket::ConnectAsync(const EndPoint& endpoint)
const noexcept
{
	co_return this->Connect(endpoint);
}

Task<SocketResult>
Socket::ConnectAsync(EndPoint&& endpoint)
const noexcept
{
	co_return this->Connect(std::move(endpoint));
}

Task<SocketResult>
Socket::ConnectAsync(const IpAddress& address, const std::uint16_t& port)
const noexcept
{
	co_return this->Connect(EndPoint(address, port));
}

Task<SocketResult>
Socket::ConnectAsync(IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	co_return this->Connect(EndPoint(address, port));
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

bool
Socket::IsAvailable()
const noexcept
{
	return myHandle != INVALID_SOCKET;
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
		return std::unexpected(AcquireSocketError());
	}
}

Socket::Socket()
noexcept
	: Socket(EmptySocket)
{}

Socket::Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept
	: Handler(sock), myProtocol(protocol), myFamily(family)
{}

[[nodiscard]]
SOCKADDR_STORAGE
SerializeEndpoint(const EndPoint& endpoint)
noexcept
{
	const auto& ip = endpoint.GetIpAddress();
	const auto& port = endpoint.GetPort();

	SOCKADDR_STORAGE result{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(result);

	switch (endpoint.GetAddressFamily())
	{
		case IpAddressFamily::IPv4:
		{
			IN_ADDR sk_addr{};
			if (not ip.TrySerialize(std::addressof(sk_addr.s_addr)))
			{
				break;
			}

			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (std::uint16_t)IpAddressFamily::IPv4,
				.sin_port = port,
				.sin_addr = std::move(sk_addr),
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case IpAddressFamily::IPv6:
		{
			IN6_ADDR sk_addr{};
			if (not ip.TrySerialize(std::addressof(sk_addr.s6_addr)))
			{
				break;
			}

			SOCKADDR_IN6 ipv6_addr
			{
				.sin6_family = (std::uint16_t)IpAddressFamily::IPv6,
				.sin6_port = port,
				.sin6_flowinfo = 0,
				.sin6_addr = std::move(sk_addr),
				.sin6_scope_id = 0,
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv6_addr));
		}
		break;

		case IpAddressFamily::Unknown:
		{}
		break;
	}

	return result;
}

[[nodiscard]]
SOCKADDR_STORAGE
SerializeEndpoint(EndPoint&& endpoint)
noexcept
{
	SOCKADDR_STORAGE result{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(result);

	switch (std::move(endpoint).GetAddressFamily())
	{
		case IpAddressFamily::IPv4:
		{
			IN_ADDR sk_addr{};
			if (not std::move(endpoint).GetIpAddress().TrySerialize(std::addressof(sk_addr.s_addr)))
			{
				break;
			}

			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (std::uint16_t)IpAddressFamily::IPv4,
				.sin_port = std::move(endpoint).GetPort(),
				.sin_addr = std::move(sk_addr),
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case IpAddressFamily::IPv6:
		{
			IN6_ADDR sk_addr{};
			if (not std::move(endpoint).GetIpAddress().TrySerialize(std::addressof(sk_addr.s6_addr)))
			{
				break;
			}

			SOCKADDR_IN6 ipv6_addr
			{
				.sin6_family = (std::uint16_t)IpAddressFamily::IPv6,
				.sin6_port = std::move(endpoint).GetPort(),
				.sin6_flowinfo = 0,
				.sin6_addr = std::move(sk_addr),
				.sin6_scope_id = 0,
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv6_addr));
		}
		break;

		case IpAddressFamily::Unknown:
		{}
		break;
	}

	return result;
}
