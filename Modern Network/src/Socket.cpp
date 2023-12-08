module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>

module Net.Socket;
import <type_traits>;
import <mutex>;
import <print>;

net::SocketOptioningResult RawSetOption(const net::NativeSocket& sock, int option, const void* buffer, int buff_size) noexcept;
net::SocketOptioningResult RawGetOption(const net::NativeSocket& sock, int option) noexcept;

using namespace net;

const Socket::EmptySocketType Socket::EmptySocket = {};
::RIO_BUF buf;
static inline constexpr ::SOCKET InvalidSocket = INVALID_SOCKET;
constinit static inline ::SOCKET internalSocket = InvalidSocket;
constinit static inline std::once_flag internalInitFlag{};

constinit static inline ::WSAOVERLAPPED rioContext{};
constinit static inline ::RIO_EXTENSION_FUNCTION_TABLE rioFunctions{};

static void CALLBACK rioRoutine(const ::DWORD err, const ::DWORD bytes, ::LPWSAOVERLAPPED ctx, const ::DWORD flags)
{
	if (0 != err)
	{
		std::println("Socket error: {}", err);
	}
}

Socket::Socket()
noexcept
	: Socket(EmptySocket)
{}

Socket::Socket(EmptySocketType)
noexcept
	: Handler(INVALID_SOCKET)
	, myProtocol(InternetProtocols::Unknown), myFamily(IpAddressFamily::Unknown)
	, IsAddressReusable(this, false, SetAddressReusable)
{}

Socket::Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept
	: Handler(sock)
	, myProtocol(protocol), myFamily(family)
	, IsAddressReusable(this, false, SetAddressReusable)
{
	std::call_once(internalInitFlag, [&]() {
		::GUID fntable_id = WSAID_MULTIPLE_RIO;
		::DWORD temp_bytes = 0;

#if _DEBUG
		const int result = 
#endif // _DEBUG
			::WSAIoctl(sock, SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER
			, std::addressof(fntable_id), sizeof(GUID)
			, reinterpret_cast<void**>(std::addressof(rioFunctions)), sizeof(rioFunctions)
			, std::addressof(temp_bytes)
			, std::addressof(rioContext), ::rioRoutine);
	});
}

Socket&
Socket::operator=(EmptySocketType)
noexcept
{
	if (IsAvailable())
	{
		Close();
	}

	return *this = Socket{ EmptySocket };
}

SocketListeningResult
Socket::Open()
const noexcept
{
	const int open = ::listen(myHandle, SOMAXCONN);
	if (SOCKET_ERROR == open)
	{
		return unexpected(AcquireListeningError());
	}

	return 0;
}

bool
Socket::Close()
noexcept
{
	if (IsAvailable())
	{
		bool result = (0 == ::closesocket(myHandle));
		myHandle = INVALID_SOCKET;

		return result;
	}
	else
	{
		return false;
	}
}

bool
Socket::Close(SocketClosingErrorCodes& error_code)
noexcept
{
	if (IsAvailable())
	{
		if (0 == ::closesocket(myHandle))
		{
			myHandle = INVALID_SOCKET;
			return true;
		}
		else
		{
			error_code = AcquireClosingError();
			return false;
		}
	}
	else
	{
		error_code = SocketClosingErrorCodes::NotASocket;
		return false;
	}
}

bool net::Socket::ReusableAddress() const noexcept
{
	return IsAddressReusable;
}

void net::Socket::ReusableAddress(bool flag) noexcept
{
	IsAddressReusable = flag;
}

Socket
Socket::Create(SocketType type
	, const InternetProtocols& protocol
	, const IpAddressFamily& family)
	noexcept
{
	const auto flags = std::to_underlying(type);

	NativeSocket result;
	switch (protocol)
	{
		case InternetProtocols::TCP:
		{
			result = ::WSASocket(static_cast<int>(family), SOCK_STREAM, ::IPPROTO::IPPROTO_TCP, nullptr, 0, flags);
		}
		break;

		case InternetProtocols::UDP:
		{
			result = ::WSASocket(static_cast<int>(family), SOCK_DGRAM, ::IPPROTO::IPPROTO_UDP, nullptr, 0, flags);
		}
		break;

		case InternetProtocols::Unknown:
		{
			return EmptySocket;
		}
	}

	return Socket(result, protocol, family);
}

Socket
Socket::Create(SocketType type
	, const InternetProtocols& protocol
	, const IpAddressFamily& family
	, ErrorCodes& error_code)
	noexcept
{
	if (Socket result = Create(type, protocol, family); result.IsAvailable())
	{
		return result;
	}
	else
	{
		error_code = AcquireNetworkError();
		return EmptySocket;
	}
}

bool
Socket::TryCreate(SocketType type
	, const InternetProtocols& protocol
	, const IpAddressFamily& family
	, AttentSocket& out)
	noexcept
{
	if (Socket result = Create(type, protocol, family); result.IsAvailable())
	{
		out = AttentSocket
		{
			.Socket = std::move(result)
		};

		return true;
	}
	else
	{
		return false;
	}
}

bool
Socket::TryCreate(SocketType type
	, const InternetProtocols& protocol
	, const IpAddressFamily& family
	, AttentSocket& out
	, ErrorCodes& error_code)
	noexcept
{
	if (Socket result = Create(type, protocol, family); result.IsAvailable())
	{
		out = AttentSocket
		{
			.Socket = std::move(result)
		};

		return true;
	}
	else
	{
		error_code = AcquireNetworkError();
		return false;
	}
}

Socket::FactoryResult
Socket::TryCreate(SocketType type
	, const InternetProtocols& protocol
	, const IpAddressFamily& family)
	noexcept
{
	if (Socket result = Create(type, protocol, family); result.IsAvailable())
	{
		return std::move(result);
	}
	else
	{
		return unexpected(AcquireNetworkError());
	}
}

void
Socket::SetAddressReusable(Socket& target, bool& flag)
noexcept
{
	::BOOL iflag = static_cast<::BOOL>(flag);

	RawSetOption(target.myHandle, SO_REUSEADDR, std::addressof(iflag), sizeof(iflag));
}

net::SocketOptioningResult
RawSetOption(const net::NativeSocket& sock, int option, const void* buffer, int buff_size)
noexcept
{
	if (0 == ::setsockopt(sock
		, SOL_SOCKET
		, option
		, reinterpret_cast<const char*>(buffer), buff_size))
	{
		return 1;
	}

	return unexpected(AcquireSocketOptionError());
}

net::SocketOptioningResult
RawGetOption(const net::NativeSocket& sock, int option)
noexcept
{
	int result = 0;
	int len = sizeof(int);

	if (0 == ::getsockopt(sock
		, SOL_SOCKET
		, option
		, reinterpret_cast<char*>(std::addressof(result)), std::addressof(len)))
	{
		return result;
	}

	return unexpected(AcquireSocketOptionError());
}
