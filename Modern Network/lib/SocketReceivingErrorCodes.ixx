export module Net.Socket.ErrorCode:Receiving;
import :General;

export namespace net
{
	enum class ReceivingErrorCodes
	{
		ConnectionAborted = (int)SocketErrorCodes::ConnectionAborted,
		ConnectionStateReset = (int)SocketErrorCodes::ConnectionStateReset,
		WSAEDISCON = (int)SocketErrorCodes::WSAEDISCON,
		WSAEFAULT = (int)SocketErrorCodes::WSAEFAULT,
		YetInProgress = (int)SocketErrorCodes::YetInProgress,
		BlockingCallsInterrupted = (int)SocketErrorCodes::BlockingCallsInterrupted,
		DidNotBind = (int)SocketErrorCodes::DidNotBind,
		WSAEMSGSIZE = (int)SocketErrorCodes::WSAEMSGSIZE,
		NetworkIsDown = (int)SocketErrorCodes::NetworkIsDown,
		WSAENETRESET = (int)SocketErrorCodes::WSAENETRESET,
		NoBufferStorage = (int)SocketErrorCodes::NoBufferStorage,
		NotConnected = (int)SocketErrorCodes::NotConnected,
		NotASocket = (int)SocketErrorCodes::NotASocket,
		CannotReceive = (int)SocketErrorCodes::NoSupportOperation,
		ClosedSocket = (int)SocketErrorCodes::ClosedSocket,
		NetworkTimeOut = (int)SocketErrorCodes::NetworkTimeOut,
		NotOverlappedOperations = (int)SocketErrorCodes::NotOverlappedOperations,
		NotInitializedSocketSystem = (int)SocketErrorCodes::NotInitializedSocketSystem,
		PendedIoOperation = (int)SocketErrorCodes::PendedIoOperation,
		OPERATION_ABORTED = (int)SocketErrorCodes::OPERATION_ABORTED,
	};
}
