export module Net.Socket.ErrorCode:Sending;
import :General;

export namespace net
{
	enum class SendingErrorCodes
	{
		WSAECONNABORTED = (int)SocketErrorCodes::WSAECONNABORTED,
		WSAECONNRESET = (int)SocketErrorCodes::WSAECONNRESET,
		WSAEFAULT = (int)SocketErrorCodes::WSAEFAULT,
		WSAEINTR = (int)SocketErrorCodes::WSAEINTR,
		WSAEINPROGRESS = (int)SocketErrorCodes::WSAEINPROGRESS,
		WSAEINVAL = (int)SocketErrorCodes::WSAEINVAL,
		WSAEMSGSIZE = (int)SocketErrorCodes::WSAEMSGSIZE,
		WSAENETDOWN = (int)SocketErrorCodes::WSAENETDOWN,
		WSAENETRESET = (int)SocketErrorCodes::WSAENETRESET,
		WSAENOBUFS = (int)SocketErrorCodes::WSAENOBUFS,
		WSAENOTCONN = (int)SocketErrorCodes::WSAENOTCONN,
		WSAENOTSOCK = (int)SocketErrorCodes::WSAENOTSOCK,
		WSAEOPNOTSUPP = (int)SocketErrorCodes::WSAEOPNOTSUPP,
		WSAESHUTDOWN = (int)SocketErrorCodes::WSAESHUTDOWN,
		WSAEWOULDBLOCK = (int)SocketErrorCodes::WSAEWOULDBLOCK,
		WSANOTINITIALISED = (int)SocketErrorCodes::WSANOTINITIALISED,
		WSA_IO_PENDING = (int)SocketErrorCodes::WSA_IO_PENDING,
		WSA_OPERATION_ABORTED = (int)SocketErrorCodes::WSA_OPERATION_ABORTED,
	};
}
