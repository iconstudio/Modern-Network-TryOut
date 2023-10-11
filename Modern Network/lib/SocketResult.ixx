export module Net.Socket:Result;
import Net.Socket.ErrorCode;
import <expected>;

export namespace net
{
	using SocketResult = std::expected<unsigned int, SocketErrorCodes>;

	using SocketReceivingResult = std::expected<unsigned int, ReceivingErrorCodes>;

	using SocketSendingResult = std::expected<unsigned int, SendingErrorCodes>;

	using SocketListeningResult = std::expected<int, SocketOpenErrorCodes>;
}
