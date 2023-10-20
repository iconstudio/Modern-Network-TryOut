export module Net.ErrorCode:Sending;
import :General;
import <cstdint>;

export namespace net
{
	enum class SendingErrorCodes : std::int32_t
	{
		ConnectionAborted = (std::int32_t)SocketErrorCodes::ConnectionAborted,
		ConnectionStateReset = (std::int32_t)SocketErrorCodes::ConnectionStateReset,
		WSAEFAULT = (std::int32_t)SocketErrorCodes::WSAEFAULT,
		BlockingCallsInterrupted = (std::int32_t)SocketErrorCodes::BlockingCallsInterrupted,
		YetInProgress = (std::int32_t)SocketErrorCodes::YetInProgress,
		WSAEINVAL = (std::int32_t)SocketErrorCodes::WSAEINVAL,
		WSAEMSGSIZE = (std::int32_t)SocketErrorCodes::WSAEMSGSIZE,
		NetworkIsDown = (std::int32_t)SocketErrorCodes::NetworkIsDown,
		WSAENETRESET = (std::int32_t)SocketErrorCodes::WSAENETRESET,
		NoBufferStorage = (std::int32_t)SocketErrorCodes::NoBufferStorage,
		NotConnected = (std::int32_t)SocketErrorCodes::NotConnected,
		NotASocket = (std::int32_t)SocketErrorCodes::NotASocket,
		CannotSend = (std::int32_t)SocketErrorCodes::NoSupportOperation,
		ClosedSocket = (std::int32_t)SocketErrorCodes::ClosedSocket,
		NonBlockedOperation = (std::int32_t)SocketErrorCodes::NonBlockedOperation,
		NotInitializedSocketSystem = (std::int32_t)SocketErrorCodes::NotInitializedSocketSystem,
		PendedIoOperation = (std::int32_t)SocketErrorCodes::PendedIoOperation,
		OPERATION_ABORTED = (std::int32_t)SocketErrorCodes::OPERATION_ABORTED,
	};
}
