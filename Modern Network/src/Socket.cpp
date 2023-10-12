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

using namespace net;

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
	: Socket()
{}

Socket&
Socket::operator=(EmptySocketType)
noexcept
{
	return *this = Socket{};
}

net::Socket::~Socket()
noexcept
{}

SocketResult
Socket::Bind(const EndPoint& endpoint)
const noexcept
{
	const auto& ip = endpoint.GetIpAddress();
	const auto port = endpoint.GetPort();
	const auto family = endpoint.GetAddressFamily();

	SOCKADDR_STORAGE sockaddr{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(sockaddr);

	SerializedIpAddress serialized_address;
	if (not ip.TrySerialize(serialized_address))
	{
		return std::unexpected(AcquireSocketError());
	}

	switch (family)
	{
		case IpAddressFamily::IPv4:
		{
			::IN_ADDR sk_addr{};
			sk_addr.S_un.S_addr = 0;
			inet_addr();

			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (std::uint16_t)family,
				.sin_port = port,
				.sin_addr = sk_addr,
			};

			sockaddr = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case IpAddressFamily::IPv6:
		{
			SOCKADDR_IN6 ipv6_addr
			{

			};

			sockaddr = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv6_addr));
		}
		break;

		case IpAddressFamily::Unknown:
		{
			return std::unexpected(AcquireSocketError());
		}
	}

	if (0 != ::bind(myHandle, reinterpret_cast<const SOCKADDR*>(sockaddr_ptr), sizeof(sockaddr)))
	{
		return std::unexpected(AcquireSocketError());
	}

	return sizeof(sockaddr);
}

SocketResult
Socket::Bind(EndPoint&& endpoint)
const noexcept
{
	return SocketResult();
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

Socket
Socket::Create()
noexcept
{
	return {};
}
