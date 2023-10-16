module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
module Net.Socket;

net::SocketReceivingResult
net::Socket::Receive(std::span<std::byte> memory)
const noexcept
{
	WSABUF buffer
	{
		.len = static_cast<ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	DWORD bytes = 0;
	DWORD flags = 0;
	if (0 == WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireReceivingError(); error != SendingErrorCodes::PendedIoOperation)
		{
			return unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}

net::SocketReceivingResult
net::Socket::Receive(const std::byte* const& memory, size_t size)
const noexcept
{
	WSABUF buffer
	{
		.len = static_cast<ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	DWORD bytes = 0;
	DWORD flags = 0;
	if (0 == WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireReceivingError(); error != SendingErrorCodes::PendedIoOperation)
		{
			return unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}
