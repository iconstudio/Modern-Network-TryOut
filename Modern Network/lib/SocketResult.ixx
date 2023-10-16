export module Net.Socket:Result;
export import Net.Socket.ErrorCode;
import <expected>;

export namespace net
{
	using SocketResult = std::expected<unsigned int, SocketErrorCodes>;

	using SocketReceivingResult = std::expected<unsigned int, ReceivingErrorCodes>;

	using SocketSendingResult = std::expected<unsigned int, SendingErrorCodes>;

	using SocketListeningResult = std::expected<int, SocketOpenErrorCodes>;

	using SocketOptioningResult = std::expected<int, SocketOptionErrorCodes>;

	using std::bad_expected_access;
	using std::expected;
	using std::unexpected;
	using std::unexpect;
	using std::unexpect_t;
}
