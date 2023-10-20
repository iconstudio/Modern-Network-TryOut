export module Net.ErrorCode:Option;
import :General;
import <cstdint>;

export namespace net
{
	enum class SocketOptionErrorCodes : std::int32_t
	{
		NotInitializedSocketSystem = (std::int32_t)SocketErrorCodes::NotInitializedSocketSystem,
		NetworkIsDown = (std::int32_t)SocketErrorCodes::NetworkIsDown,
		BlockingCallsInterrupted = (std::int32_t)SocketErrorCodes::BlockingCallsInterrupted,

		InvalidBuffer = (std::int32_t)SocketErrorCodes::WSAEFAULT,
		InvalidValueParameters = (std::int32_t)SocketErrorCodes::WSAEINVAL,
		YetInProgress = (std::int32_t)SocketErrorCodes::YetInProgress,
		WSAENETRESET = (std::int32_t)SocketErrorCodes::WSAENETRESET,
		WSAENOPROTOOPT = (std::int32_t)SocketErrorCodes::WSAENOPROTOOPT,
		NotConnected = (std::int32_t)SocketErrorCodes::NotConnected,
		NotASocket = (std::int32_t)SocketErrorCodes::NotASocket,
	};
}
