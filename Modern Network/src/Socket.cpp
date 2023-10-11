module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <type_traits>
module Net.Socket;

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

constexpr
Socket::Socket(EmptySocketType)
noexcept
	: Socket()
{}

constexpr Socket&
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
