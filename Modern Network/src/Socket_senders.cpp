module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

module Net.Socket;

net::SocketSendingResult RawSend(const net::NativeSocket& sock, ::WSABUF& buffer) noexcept;
net::SocketSendingResult RawSendEx(const net::NativeSocket& sock, ::WSABUF& buffer, void* context, ::LPWSAOVERLAPPED_COMPLETION_ROUTINE routine) noexcept;

net::SocketSendingResult
net::Socket::Send(std::span<const std::byte> memory)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	return RawSend(myHandle, buffer);
}

net::SocketSendingResult
net::Socket::Send(const std::byte* const& memory, size_t size)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	return RawSend(myHandle, buffer);
}

bool
net::Socket::Send(std::span<const std::byte> memory
	, net::SendingErrorCodes& error_code)
const noexcept
{
	return Send(memory).and_then(
		[](unsigned int&&) noexcept -> expected<bool, SendingErrorCodes> {
		return true;
	}).or_else(
		[&](SendingErrorCodes&& tr_error_code) noexcept -> expected<bool, SendingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

bool
net::Socket::Send(const std::byte* const& memory, size_t size
	, net::SendingErrorCodes& error_code)
const noexcept
{
	return Send(memory, size).and_then(
		[](unsigned int&&) noexcept -> expected<bool, SendingErrorCodes> {
		return true;
	}).or_else(
		[&](SendingErrorCodes&& tr_error_code) noexcept -> expected<bool, SendingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

net::SocketSendingResult
net::Socket::Send(net::IoContext* context
	, std::span<const std::byte> memory)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(memory.size_bytes()),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory.data())),
	};

	return RawSendEx(myHandle, buffer, context, nullptr);
}

net::SocketSendingResult
net::Socket::Send(net::IoContext* context
	, const std::byte* const& memory, size_t size)
const noexcept
{
	::WSABUF buffer
	{
		.len = static_cast<::ULONG>(size),
		.buf = reinterpret_cast<char*>(const_cast<std::byte*>(memory)),
	};

	return RawSendEx(myHandle, buffer, context, nullptr);
}

bool
net::Socket::Send(net::IoContext* context
	, std::span<const std::byte> memory
	, net::SendingErrorCodes& error_code)
const noexcept
{
	return Send(context, memory).and_then(
		[](unsigned int&&) noexcept -> expected<bool, SendingErrorCodes> {
		return true;
	}).or_else(
		[&](SendingErrorCodes&& tr_error_code) noexcept -> expected<bool, SendingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

bool
net::Socket::Send(net::IoContext* context
	, const std::byte* const& memory
	, size_t size, net::SendingErrorCodes& error_code)
const noexcept
{
	return Send(context, memory, size).and_then(
		[](unsigned int&&) noexcept -> expected<bool, SendingErrorCodes> {
		return true;
	}).or_else(
		[&](SendingErrorCodes&& tr_error_code) noexcept -> expected<bool, SendingErrorCodes> {
		error_code = std::move(tr_error_code);
		return false;
	}).value_or(false);
}

net::SocketSendingResult
RawSend(const net::NativeSocket& sock
	, ::WSABUF& buffer)
noexcept
{
	if (::DWORD bytes = 0; 0 == ::WSASend(sock
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, 0
		, nullptr, nullptr))
	{
		return bytes;
	}
	else
	{
		return net::unexpected(net::AcquireSendingError());
	}
}

net::SocketSendingResult
RawSendEx(const net::NativeSocket& sock
	, ::WSABUF& buffer
	, void* context
	, ::LPWSAOVERLAPPED_COMPLETION_ROUTINE routine)
noexcept
{
	if (::DWORD bytes = 0; 0 == ::WSASend(sock
		, std::addressof(buffer), 1
		, std::addressof(bytes)
		, 0
		, reinterpret_cast<::LPWSAOVERLAPPED>(context)
		, routine))
	{
		return bytes;
	}
	else
	{
		if (net::SendingErrorCodes error = net::AcquireSendingError(); error != net::SendingErrorCodes::PendedIoOperation)
		{
			return net::unexpected(std::move(error));
		}
		else
		{
			return 0U;
		}
	}
}
