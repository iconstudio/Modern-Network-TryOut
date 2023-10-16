export module Net.Socket.ErrorCode:Open;
import :General;
import <cstdint>;

export namespace net
{
	enum class SocketOpenErrorCodes : std::int32_t
	{
		NotInitializedSocketSystem = (std::int32_t)SocketErrorCodes::NotInitializedSocketSystem,
		NetworkIsDown = (std::int32_t)SocketErrorCodes::NetworkIsDown,
		CannotUseThisAddress = (std::int32_t)SocketErrorCodes::CannotUseThisAddress,
		YetInProgress = (std::int32_t)SocketErrorCodes::YetInProgress,
		WSAEINVAL = (std::int32_t)SocketErrorCodes::WSAEINVAL,
		AlreadyConnected = (std::int32_t)SocketErrorCodes::WSAEISCONN,
		NotASocket = (std::int32_t)SocketErrorCodes::NotASocket,
		CouldNotUseThis = (std::int32_t)SocketErrorCodes::WSAEMFILE,
		NoBufferStorage = (std::int32_t)SocketErrorCodes::NoBufferStorage,
		NoSupportOperation = (std::int32_t)SocketErrorCodes::NoSupportOperation
	};
}
