module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <ws2ipdef.h>
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
	const auto addr = endpoint.GetAddress();
	const auto port = endpoint.GetPort();
	const auto family = endpoint.GetAddressFamily();
	const auto ip_type = endpoint.GetType();

	SOCKADDR_STORAGE sockaddr{};
	switch (ip_type)
	{
		case IpAddressType::IPv4:
		{
			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (short) std::uint16_t,
				.sin_addr.s_addr = 0, //TODO
				.sin_port = port,
			};

			sockaddr = *reinterpret_cast<SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case IpAddressType::IPv6:
		{
			SOCKADDR_IN6 ipv6_addr
			{

			};

			sockaddr = *reinterpret_cast<SOCKADDR_STORAGE*>(&std::addressof(ipv6_addr));
		}
		break;

		case IpAddressType::Unknown:
		{
			return std::unexpected(AcquireSocketError());
		}
	}

	const int result = ::bind(myHandle, std::addressof(sockaddr), sizeof(sockaddr));

	return SocketResult();
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
