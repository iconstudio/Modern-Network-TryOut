export module Net.Socket.ErrorCode:Open;
import :General;

export namespace net
{
	enum class SocketOpenErrorCodes
	{
		NotInitializedSocketSystem = (int)SocketErrorCodes::NotInitializedSocketSystem,
		NetworkIsDown = (int)SocketErrorCodes::NetworkIsDown,
		CannotUseThisAddress = (int)SocketErrorCodes::CannotUseThisAddress,
		YetInProgress = (int)SocketErrorCodes::YetInProgress,
		DidNotBind = (int)SocketErrorCodes::DidNotBind,
		AlreadyConnected = (int)SocketErrorCodes::WSAEISCONN,
		NotASocket = (int)SocketErrorCodes::NotASocket,
		CouldNotUseThis = (int)SocketErrorCodes::WSAEMFILE,
		NoBufferStorage = (int)SocketErrorCodes::NoBufferStorage,
		NoSupportOperation = (int)SocketErrorCodes::NoSupportOperation
	};
}
