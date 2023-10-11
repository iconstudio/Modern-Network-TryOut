export module Net.Socket.ErrorCode:Sending;
import :General;

export namespace net
{
	enum class SendingErrorCodes
	{
		ConnectionAborted = (int)SocketErrorCodes::ConnectionAborted,
		ConnectionStateReset = (int)SocketErrorCodes::ConnectionStateReset,
		WSAEFAULT = (int)SocketErrorCodes::WSAEFAULT,
		BlockingCallsInterrupted = (int)SocketErrorCodes::BlockingCallsInterrupted,
		YetInProgress = (int)SocketErrorCodes::YetInProgress,
		DidNotBind = (int)SocketErrorCodes::DidNotBind,
		WSAEMSGSIZE = (int)SocketErrorCodes::WSAEMSGSIZE,
		NetworkIsDown = (int)SocketErrorCodes::NetworkIsDown,
		WSAENETRESET = (int)SocketErrorCodes::WSAENETRESET,
		NoBufferStorage = (int)SocketErrorCodes::NoBufferStorage,
		NotConnected = (int)SocketErrorCodes::NotConnected,
		NotASocket = (int)SocketErrorCodes::NotASocket,
		CannotSend = (int)SocketErrorCodes::NoSupportOperation,
		ClosedSocket = (int)SocketErrorCodes::ClosedSocket,
		NotOverlappedOperations = (int)SocketErrorCodes::NotOverlappedOperations,
		NotInitializedSocketSystem = (int)SocketErrorCodes::NotInitializedSocketSystem,
		PendedIoOperation = (int)SocketErrorCodes::PendedIoOperation,
		OPERATION_ABORTED = (int)SocketErrorCodes::OPERATION_ABORTED,
	};
}
