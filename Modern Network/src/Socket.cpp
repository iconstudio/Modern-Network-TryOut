module Net.Socket;

using namespace net;

SocketSendingResult Socket::Send(std::span<const std::byte> buffer) const noexcept
{
	return SocketSendingResult(0);
}
