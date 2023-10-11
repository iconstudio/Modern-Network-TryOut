export module Net.Socket.ErrorCode:Open;
import :General;

export namespace net
{
	enum class SocketOpenErrorCodes
	{
		WSANOTINITIALISED = (int)SocketErrorCodes::WSANOTINITIALISED,
		NETDOWN = (int)SocketErrorCodes::NETDOWN,
		WSAEADDRINUSE = (int)SocketErrorCodes::WSAEADDRINUSE,
		WSAEINPROGRESS = (int)SocketErrorCodes::WSAEINPROGRESS,
		WSAEINVAL = (int)SocketErrorCodes::WSAEINVAL,
		WSAEISCONN = (int)SocketErrorCodes::WSAEISCONN,
		WSAENOTSOCK = (int)SocketErrorCodes::WSAENOTSOCK,
		WSAEMFILE = (int)SocketErrorCodes::WSAEMFILE,
		WSAENOBUFS = (int)SocketErrorCodes::WSAENOBUFS,
		WSAEOPNOTSUPP = (int)SocketErrorCodes::WSAEOPNOTSUPP
	};
}
