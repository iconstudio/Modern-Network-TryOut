export module Net.Socket.ErrorCode:Receiving;
import :General;

export namespace net
{
	enum class ReceivingErrorCodes
	{
		WSAECONNABORTED = (int)SocketErrorCodes::WSAECONNABORTED,
		WSAECONNRESET = (int)SocketErrorCodes::WSAECONNRESET,
		WSAEDISCON = (int)SocketErrorCodes::WSAEDISCON,
		WSAEFAULT = (int)SocketErrorCodes::WSAEFAULT,
		WSAEINPROGRESS = (int)SocketErrorCodes::WSAEINPROGRESS,
		WSAEINTR = (int)SocketErrorCodes::WSAEINTR,
		WSAEINVAL = (int)SocketErrorCodes::WSAEINVAL,
		WSAEMSGSIZE = (int)SocketErrorCodes::WSAEMSGSIZE,
		NETDOWN = (int)SocketErrorCodes::NETDOWN,
		WSAENETRESET = (int)SocketErrorCodes::WSAENETRESET,
		WSAENOBUFS = (int)SocketErrorCodes::WSAENOBUFS,
		WSAENOTCONN = (int)SocketErrorCodes::WSAENOTCONN,
		WSAENOTSOCK = (int)SocketErrorCodes::WSAENOTSOCK,
		WSAEOPNOTSUPP = (int)SocketErrorCodes::WSAEOPNOTSUPP,
		WSAESHUTDOWN = (int)SocketErrorCodes::WSAESHUTDOWN,
		WSAETIMEDOUT = (int)SocketErrorCodes::WSAETIMEDOUT,
		WSAEWOULDBLOCK = (int)SocketErrorCodes::WSAEWOULDBLOCK,
		WSANOTINITIALISED = (int)SocketErrorCodes::WSANOTINITIALISED,
		IO_PENDING = (int)SocketErrorCodes::IO_PENDING,
		OPERATION_ABORTED = (int)SocketErrorCodes::OPERATION_ABORTED,
	};
}
