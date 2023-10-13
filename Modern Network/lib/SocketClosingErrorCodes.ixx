export module Net.Socket.ErrorCode:Closing;
import :General;

export namespace net
{
	enum class SocketClosingErrorCodes
	{
		NotInitializedSocketSystem = (int)SocketErrorCodes::NotInitializedSocketSystem,
		NetworkIsDown = (int)SocketErrorCodes::NetworkIsDown,
		NotASocket = (int)SocketErrorCodes::NotASocket,
		YetInProgress = (int)SocketErrorCodes::YetInProgress,
		BlockingCallsInterrupted = (int)SocketErrorCodes::BlockingCallsInterrupted,
		NotOverlappedOperations = (int)SocketErrorCodes::NotOverlappedOperations,
	};
}
