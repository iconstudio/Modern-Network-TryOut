module;
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
	int result = WSASend(reinterpret_cast<SOCKET>(myHandle.GetPointer()), std::addressof(buffer), 1, std::addressof(bytes), 0, nullptr, nullptr);
	if (0 == result)
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireSendingError(); error != SendingErrorCodes::IO_PENDING)
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
net::Socket::Socket(EmptySocketType)
noexcept
	: Socket()
{}

constexpr Socket&
net::Socket::operator=(EmptySocketType)
noexcept
{
	return *this = Socket{};
}

Socket
Socket::Create()
noexcept
{
	return {};
}
