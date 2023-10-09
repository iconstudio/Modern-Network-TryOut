module;
module Net.Socket;

using namespace net;

const Socket::EmptySocketType Socket::EmptySocket = {};

SocketSendingResult
Socket::Send(std::span<const std::byte> buffer)
const noexcept
{
	return SocketSendingResult(0);
}

SocketSendingResult
Socket::Send(const std::byte* const& buffer, size_t size)
const noexcept
{
	return SocketSendingResult();
}

SocketReceivingResult
Socket::Receive(std::span<std::byte> buffer)
const noexcept
{
	return SocketReceivingResult();
}

SocketReceivingResult
Socket::Receive(const std::byte* const& buffer, size_t size)
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
