export module Net.Socket.ErrorCode:Closing;
import :General;
import <cstdint>;

export namespace net
{
	enum class SocketClosingErrorCodes : std::int32_t
	{
		NotInitializedSocketSystem = (std::int32_t)SocketErrorCodes::NotInitializedSocketSystem,
		NetworkIsDown = (std::int32_t)SocketErrorCodes::NetworkIsDown,
		NotASocket = (std::int32_t)SocketErrorCodes::NotASocket,
		YetInProgress = (std::int32_t)SocketErrorCodes::YetInProgress,
		BlockingCallsInterrupted = (std::int32_t)SocketErrorCodes::BlockingCallsInterrupted,
		NotOverlappedOperations = (std::int32_t)SocketErrorCodes::NotOverlappedOperations,
	};
}
