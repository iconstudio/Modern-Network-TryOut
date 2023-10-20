module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

module Net.Socket;

net::SocketReceivingResult
net::Socket::Receive(std::span<std::byte> memory)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	::DWORD bytes = 0;
	::DWORD flags = 0;
	if (0 == ::WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		return unexpected(AcquireReceivingError());
	}
}

net::SocketReceivingResult
net::Socket::Receive(const std::byte* const& memory, size_t size)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	::DWORD bytes = 0;
	::DWORD flags = 0;
	if (0 == ::WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		return unexpected(AcquireReceivingError());
	}
}

bool
net::Socket::Receive(std::span<std::byte> memory
	, net::ReceivingErrorCodes& error_code)
	const noexcept
{
	return Receive(memory).and_then(
		[](unsigned int&&) noexcept -> expected<bool, ReceivingErrorCodes> {
		return true;
	}).or_else(
		[&](net::ReceivingErrorCodes&& tr_error_code) noexcept -> expected<bool, ReceivingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

bool
net::Socket::Receive(const std::byte* const& memory, size_t size
	, net::ReceivingErrorCodes& error_code)
	const noexcept
{
	return Receive(memory, size).and_then(
		[](unsigned int&&) noexcept -> expected<bool, ReceivingErrorCodes> {
		return true;
	}).or_else(
		[&](net::ReceivingErrorCodes&& tr_error_code) noexcept -> expected<bool, ReceivingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

net::SocketReceivingResult
net::Socket::Receive(IoContext* context
	, std::span<std::byte> memory)
	const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	::DWORD bytes = 0;
	::DWORD flags = 0;
	if (0 == ::WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, reinterpret_cast<::LPWSAOVERLAPPED>(context)
		, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireReceivingError(); error != SocketErrorCodes::PendedIoOperation)
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
net::Socket::Receive(IoContext* context
	, const std::byte* const& memory, size_t size)
	const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	::DWORD bytes = 0;
	::DWORD flags = 0;
	if (0 == ::WSARecv(myHandle
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, std::addressof(flags)
		, reinterpret_cast<::LPWSAOVERLAPPED>(context)
		, nullptr))
	{
		return bytes;
	}
	else
	{
		if (auto error = AcquireReceivingError(); error != SocketErrorCodes::PendedIoOperation)
		{
			return unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}

bool
net::Socket::Receive(IoContext* context
	, std::span<std::byte> memory
	, ReceivingErrorCodes& error_code)
	const noexcept
{
	return Receive(context, memory).and_then(
		[](unsigned int&&) noexcept -> expected<bool, ReceivingErrorCodes> {
		return true;
	}).or_else(
		[&](net::ReceivingErrorCodes&& tr_error_code) noexcept -> expected<bool, ReceivingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

bool
net::Socket::Receive(IoContext* context
	, const std::byte* const& memory, size_t size
	, ReceivingErrorCodes& error_code)
	const noexcept
{
	return Receive(context, memory, size).and_then(
		[](unsigned int&&) noexcept -> expected<bool, ReceivingErrorCodes> {
		return true;
	}).or_else(
		[&](net::ReceivingErrorCodes&& tr_error_code) noexcept -> expected<bool, ReceivingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

net::Task<net::SocketReceivingResult>
net::Socket::ReceiveAsync(net::IoContext* context
	, std::span<std::byte> memory)
	const noexcept
{
	if (SocketReceivingResult sent = Receive(context, memory); not sent)
	{
		co_return std::move(sent);
	}

	static ::DWORD flags = 0;
	::DWORD transferred_bytes = 0;

	::BOOL result = ::WSAGetOverlappedResult(myHandle
		, reinterpret_cast<::LPWSAOVERLAPPED>(context)
		, std::addressof(transferred_bytes)
		, TRUE
		, std::addressof(flags));

	if (FALSE == result)
	{
		co_return net::unexpected(net::AcquireReceivingError());
	}
	else
	{
		co_return transferred_bytes;
	}
}

net::Task<net::SocketReceivingResult>
net::Socket::ReceiveAsync(net::IoContext* context
	, const std::byte* const& memory, size_t size)
	const noexcept
{
	if (SocketReceivingResult sent = Receive(context, memory, size); not sent)
	{
		co_return std::move(sent);
	}

	static ::DWORD flags = 0;
	::DWORD transferred_bytes = 0;

	::BOOL result = ::WSAGetOverlappedResult(myHandle
		, reinterpret_cast<::LPWSAOVERLAPPED>(context)
		, std::addressof(transferred_bytes)
		, TRUE
		, std::addressof(flags));

	if (FALSE == result)
	{
		co_return net::unexpected(net::AcquireReceivingError());
	}
	else
	{
		co_return transferred_bytes;
	}
}
