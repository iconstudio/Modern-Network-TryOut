export module Net.Socket.ErrorCode:Receiving;
import :General;
import <cstdint>;

export namespace net
{
	enum class ReceivingErrorCodes : std::int32_t
	{
		ConnectionAborted = (std::int32_t)SocketErrorCodes::ConnectionAborted,
		ConnectionStateReset = (std::int32_t)SocketErrorCodes::ConnectionStateReset,
		WSAEDISCON = (std::int32_t)SocketErrorCodes::WSAEDISCON,
		WSAEFAULT = (std::int32_t)SocketErrorCodes::WSAEFAULT,
		YetInProgress = (std::int32_t)SocketErrorCodes::YetInProgress,
		BlockingCallsInterrupted = (std::int32_t)SocketErrorCodes::BlockingCallsInterrupted,
		WSAEINVAL = (std::int32_t)SocketErrorCodes::WSAEINVAL,
		WSAEMSGSIZE = (std::int32_t)SocketErrorCodes::WSAEMSGSIZE,
		NetworkIsDown = (std::int32_t)SocketErrorCodes::NetworkIsDown,
		WSAENETRESET = (std::int32_t)SocketErrorCodes::WSAENETRESET,
		NoBufferStorage = (std::int32_t)SocketErrorCodes::NoBufferStorage,
		NotConnected = (std::int32_t)SocketErrorCodes::NotConnected,
		NotASocket = (std::int32_t)SocketErrorCodes::NotASocket,
		CannotReceive = (std::int32_t)SocketErrorCodes::NoSupportOperation,
		ClosedSocket = (std::int32_t)SocketErrorCodes::ClosedSocket,
		NetworkTimeOut = (std::int32_t)SocketErrorCodes::NetworkTimeOut,
		NonBlockedOperation = (std::int32_t)SocketErrorCodes::NonBlockedOperation,
		NotInitializedSocketSystem = (std::int32_t)SocketErrorCodes::NotInitializedSocketSystem,
		PendedIoOperation = (std::int32_t)SocketErrorCodes::PendedIoOperation,
		OPERATION_ABORTED = (std::int32_t)SocketErrorCodes::OPERATION_ABORTED,
	};
}
