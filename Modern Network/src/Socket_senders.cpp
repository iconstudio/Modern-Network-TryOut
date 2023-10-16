module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
module Net.Socket;

net::SocketSendingResult
net::Socket::Send(std::span<const std::byte> memory)
const noexcept
{
	WSABUF buffer
	{
		.len = static_cast<ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	DWORD bytes = 0;
	if (0 == WSASend(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, 0
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireSendingError(); error != SendingErrorCodes::PendedIoOperation)
		{
			return unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}

net::SocketSendingResult
net::Socket::Send(const std::byte* const& memory, size_t size)
const noexcept
{
	WSABUF buffer
	{
		.len = static_cast<ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	DWORD bytes = 0;
	if (0 == WSASend(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, 0
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireSendingError(); error != SendingErrorCodes::PendedIoOperation)
		{
			return unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}
