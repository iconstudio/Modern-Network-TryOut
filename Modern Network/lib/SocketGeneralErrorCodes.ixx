export module Net.ErrorCode:General;
import <cstdint>;
import <string>;
import <format>;

export namespace net
{
	enum class ErrorCodes : std::int32_t
	{
		PendedIoOperation = 997,
		WSA_IO_INCOMPLETE = 996,
		WSA_INVALID_HANDLE = 6,
		WSA_INVALID_PARAMETER = 87,
		WSA_NOT_ENOUGH_MEMORY = 8,
		OPERATION_ABORTED = 995,
		WSA_WAIT_EVENT_0 = 0,
		WSA_WAIT_IO_COMPLETION = 192,
		WSABASEERR = 10000,
		BlockingCallsInterrupted = 10004,
		WSAEBADF = 10009,
		WSAEACCES = 10013,
		WSAEFAULT = 10014,
		WSAEINVAL = 10022,
		WSAEMFILE = 10024,
		NonBlockedOperation = 10035, // EWOULDBLOCK
		YetInProgress = 10036,
		WSAEALREADY = 10037,
		NotASocket = 10038,
		WSAEDESTADDRREQ = 10039,
		WSAEMSGSIZE = 10040,
		WSAEPROTOTYPE = 10041,
		WSAENOPROTOOPT = 10042,
		WSAEPROTONOSUPPORT = 10043,
		WSAESOCKTNOSUPPORT = 10044,
		NoSupportOperation = 10045,
		WSAEPFNOSUPPORT = 10046,
		WSAEAFNOSUPPORT = 10047,
		CannotUseThisAddress = 10048,
		WSAEADDRNOTAVAIL = 10049,
		NetworkIsDown = 10050,
		WSAENETUNREACH = 10051,
		WSAENETRESET = 10052,
		ConnectionAborted = 10053,
		ConnectionStateReset = 10054,
		NoBufferStorage = 10055,
		WSAEISCONN = 10056,
		NotConnected = 10057,
		ClosedSocket = 10058,
		WSAETOOMANYREFS = 10059,
		NetworkTimeOut = 10060,
		WSAECONNREFUSED = 10061,
		WSAELOOP = 10062,
		WSAENAMETOOLONG = 10063,
		WSAEHOSTDOWN = 10064,
		WSAEHOSTUNREACH = 10065,
		WSAENOTEMPTY = 10066,
		WSAEPROCLIM = 10067,
		WSAEUSERS = 10068,
		WSAEDQUOT = 10069,
		WSAESTALE = 10070,
		WSAEREMOTE = 10071,
		WSASYSNOTREADY = 10091,
		WSAVERNOTSUPPORTED = 10092,
		NotInitializedSocketSystem = 10093,
		WSAEDISCON = 10101,
		WSAENOMORE = 10102,
		WSAECANCELLED = 10103,
		WSAEINVALIDPROCTABLE = 10104,
		WSAEINVALIDPROVIDER = 10105,
		WSAEPROVIDERFAILEDINIT = 10106,
		WSASYSCALLFAILURE = 10107,
		WSASERVICE_NOT_FOUND = 10108,
		WSATYPE_NOT_FOUND = 10109,
		WSA_E_NO_MORE = 10110,
		WSA_E_CANCELLED = 10111,
		WSAEREFUSED = 10112,
		WSAHOST_NOT_FOUND = 11001,
		WSATRY_AGAIN = 11002,
		WSANO_RECOVERY = 11003,
		WSANO_DATA = 11004,
		WSA_QOS_RECEIVERS = 11005,
		WSA_QOS_SENDERS = 11006,
		WSA_QOS_NO_SENDERS = 11007,
		WSA_QOS_NO_RECEIVERS = 11008,
		WSA_QOS_REQUEST_CONFIRMED = 11009,
		WSA_QOS_ADMISSION_FAILURE = 11010,
		WSA_QOS_POLICY_FAILURE = 11011,
		WSA_QOS_BAD_STYLE = 11012,
		WSA_QOS_BAD_OBJECT = 11013,
		WSA_QOS_TRAFFIC_CTRL_ERROR = 11014,
		WSA_QOS_GENERIC_ERROR = 11015,
		WSA_QOS_ESERVICETYPE = 11016,
		WSA_QOS_EFLOWSPEC = 11017,
		WSA_QOS_EPROVSPECBUF = 11018,
		WSA_QOS_EFILTERSTYLE = 11019,
		WSA_QOS_EFILTERTYPE = 11020,
		WSA_QOS_EFILTERCOUNT = 11021,
		WSA_QOS_EOBJLENGTH = 11022,
		WSA_QOS_EFLOWCOUNT = 11023,
		WSA_QOS_EUNKOWNPSOBJ = 11024,
		WSA_QOS_EPOLICYOBJ = 11025,
		WSA_QOS_EFLOWDESC = 11026,
		WSA_QOS_EPSFLOWSPEC = 11027,
		WSA_QOS_EPSFILTERSPEC = 11028,
		WSA_QOS_ESDMODEOBJ = 11029,
		WSA_QOS_ESHAPERATEOBJ = 11030,
		WSA_QOS_RESERVED_PETYPE = 11031,
		WSA_SECURE_HOST_NOT_FOUND = 11032,
		WSA_IPSEC_NAME_POLICY_ERROR = 11033,
	};
}

export namespace std
{
	[[nodiscard]]
	constexpr std::string to_string(const net::ErrorCodes& err)
	{
		using namespace std::string_literals;

		switch (err)
		{
			case net::ErrorCodes::PendedIoOperation:
			{
				return "PendedIoOperation"s;
			}

			case net::ErrorCodes::WSA_IO_INCOMPLETE:
			{
				return "WSA_IO_INCOMPLETE"s;
			}

			case net::ErrorCodes::WSA_INVALID_HANDLE:
			{
				return "WSA_INVALID_HANDLE"s;
			}

			case net::ErrorCodes::WSA_INVALID_PARAMETER:
			{
				return "WSA_INVALID_PARAMETER"s;
			}

			case net::ErrorCodes::WSA_NOT_ENOUGH_MEMORY:
			{
				return "WSA_NOT_ENOUGH_MEMORY"s;
			}

			case net::ErrorCodes::OPERATION_ABORTED:
			{
				return "OPERATION_ABORTED"s;
			}

			case net::ErrorCodes::WSA_WAIT_EVENT_0:
			{
				return "WSA_WAIT_EVENT_0"s;
			}

			case net::ErrorCodes::WSA_WAIT_IO_COMPLETION:
			{
				return "WSA_WAIT_IO_COMPLETION"s;
			}

			case net::ErrorCodes::WSABASEERR:
			{
				return "WSABASEERR"s;
			}

			case net::ErrorCodes::BlockingCallsInterrupted:
			{
				return "BlockingCallsInterrupted"s;
			}

			case net::ErrorCodes::WSAEBADF:
			{
				return "WSAEBADF"s;
			}

			case net::ErrorCodes::WSAEACCES:
			{
				return "WSAEACCES"s;
			}

			case net::ErrorCodes::WSAEFAULT:
			{
				return "WSAEFAULT"s;
			}

			case net::ErrorCodes::WSAEINVAL:
			{
				return "WSAEINVAL"s;
			}

			case net::ErrorCodes::WSAEMFILE:
			{
				return "WSAEMFILE"s;
			}

			case net::ErrorCodes::NonBlockedOperation:
			{
				return "NonBlockedOperation"s;
			}

			case net::ErrorCodes::YetInProgress:
			{
				return "YetInProgress"s;
			}

			case net::ErrorCodes::WSAEALREADY:
			{
				return "WSAEALREADY"s;
			}

			case net::ErrorCodes::NotASocket:
			{
				return "NotASocket"s;
			}

			case net::ErrorCodes::WSAEDESTADDRREQ:
			{
				return "WSAEDESTADDRREQ"s;
			}

			case net::ErrorCodes::WSAEMSGSIZE:
			{
				return "WSAEMSGSIZE"s;
			}

			case net::ErrorCodes::WSAEPROTOTYPE:
			{
				return "WSAEPROTOTYPE"s;
			}

			case net::ErrorCodes::WSAENOPROTOOPT:
			{
				return "WSAENOPROTOOPT"s;
			}

			case net::ErrorCodes::WSAEPROTONOSUPPORT:
			{
				return "WSAEPROTONOSUPPORT"s;
			}

			case net::ErrorCodes::WSAESOCKTNOSUPPORT:
			{
				return "WSAESOCKTNOSUPPORT"s;
			}

			case net::ErrorCodes::NoSupportOperation:
			{
				return "NoSupportOperation"s;
			}

			case net::ErrorCodes::WSAEPFNOSUPPORT:
			{
				return "WSAEPFNOSUPPORT"s;
			}

			case net::ErrorCodes::WSAEAFNOSUPPORT:
			{
				return "WSAEAFNOSUPPORT"s;
			}

			case net::ErrorCodes::CannotUseThisAddress:
			{
				return "CannotUseThisAddress"s;
			}

			case net::ErrorCodes::WSAEADDRNOTAVAIL:
			{
				return "WSAEADDRNOTAVAIL"s;
			}

			case net::ErrorCodes::NetworkIsDown:
			{
				return "NetworkIsDown"s;
			}

			case net::ErrorCodes::WSAENETUNREACH:
			{
				return "WSAENETUNREACH"s;
			}

			case net::ErrorCodes::WSAENETRESET:
			{
				return "WSAENETRESET"s;
			}

			case net::ErrorCodes::ConnectionAborted:
			{
				return "ConnectionAborted"s;
			}

			case net::ErrorCodes::ConnectionStateReset:
			{
				return "ConnectionStateReset"s;
			}

			case net::ErrorCodes::NoBufferStorage:
			{
				return "NoBufferStorage"s;
			}

			case net::ErrorCodes::WSAEISCONN:
			{
				return "WSAEISCONN"s;
			}

			case net::ErrorCodes::NotConnected:
			{
				return "NotConnected"s;
			}

			case net::ErrorCodes::ClosedSocket:
			{
				return "ClosedSocket"s;
			}

			case net::ErrorCodes::WSAETOOMANYREFS:
			{
				return "WSAETOOMANYREFS"s;
			}

			case net::ErrorCodes::NetworkTimeOut:
			{
				return "NetworkTimeOut"s;
			}

			case net::ErrorCodes::WSAECONNREFUSED:
			{
				return "WSAECONNREFUSED"s;
			}

			case net::ErrorCodes::WSAELOOP:
			{
				return "WSAELOOP"s;
			}

			case net::ErrorCodes::WSAENAMETOOLONG:
			{
				return "WSAENAMETOOLONG"s;
			}

			case net::ErrorCodes::WSAEHOSTDOWN:
			{
				return "WSAEHOSTDOWN"s;
			}

			case net::ErrorCodes::WSAEHOSTUNREACH:
			{
				return "WSAEHOSTUNREACH"s;
			}

			case net::ErrorCodes::WSAENOTEMPTY:
			{
				return "WSAENOTEMPTY"s;
			}

			case net::ErrorCodes::WSAEPROCLIM:
			{
				return "WSAEPROCLIM"s;
			}

			case net::ErrorCodes::WSAEUSERS:
			{
				return "WSAEUSERS"s;
			}

			case net::ErrorCodes::WSAEDQUOT:
			{
				return "WSAEDQUOT"s;
			}

			case net::ErrorCodes::WSAESTALE:
			{
				return "WSAESTALE"s;
			}

			case net::ErrorCodes::WSAEREMOTE:
			{
				return "WSAEREMOTE"s;
			}

			case net::ErrorCodes::WSASYSNOTREADY:
			{
				return "WSASYSNOTREADY"s;
			}

			case net::ErrorCodes::WSAVERNOTSUPPORTED:
			{
				return "WSAVERNOTSUPPORTED"s;
			}

			case net::ErrorCodes::NotInitializedSocketSystem:
			{
				return "NotInitializedSocketSystem"s;
			}

			case net::ErrorCodes::WSAEDISCON:
			{
				return "WSAEDISCON"s;
			}

			case net::ErrorCodes::WSAENOMORE:
			{
				return "WSAENOMORE"s;
			}

			case net::ErrorCodes::WSAECANCELLED:
			{
				return "WSAECANCELLED"s;
			}

			case net::ErrorCodes::WSAEINVALIDPROCTABLE:
			{
				return "WSAEINVALIDPROCTABLE"s;
			}

			case net::ErrorCodes::WSAEINVALIDPROVIDER:
			{
				return "WSAEINVALIDPROVIDER"s;
			}

			case net::ErrorCodes::WSAEPROVIDERFAILEDINIT:
			{
				return "WSAEPROVIDERFAILEDINIT"s;
			}

			case net::ErrorCodes::WSASYSCALLFAILURE:
			{
				return "WSASYSCALLFAILURE"s;
			}

			case net::ErrorCodes::WSASERVICE_NOT_FOUND:
			{
				return "WSASERVICE_NOT_FOUND"s;
			}

			case net::ErrorCodes::WSATYPE_NOT_FOUND:
			{
				return "WSATYPE_NOT_FOUND"s;
			}

			case net::ErrorCodes::WSA_E_NO_MORE:
			{
				return "WSA_E_NO_MORE"s;
			}

			case net::ErrorCodes::WSA_E_CANCELLED:
			{
				return "WSA_E_CANCELLED"s;
			}

			case net::ErrorCodes::WSAEREFUSED:
			{
				return "WSAEREFUSED"s;
			}

			case net::ErrorCodes::WSAHOST_NOT_FOUND:
			{
				return "WSAHOST_NOT_FOUND"s;
			}

			case net::ErrorCodes::WSATRY_AGAIN:
			{
				return "WSATRY_AGAIN"s;
			}

			case net::ErrorCodes::WSANO_RECOVERY:
			{
				return "WSANO_RECOVERY"s;
			}

			case net::ErrorCodes::WSANO_DATA:
			{
				return "WSANO_DATA"s;
			}

			default:
			{
				return std::to_string(static_cast<int>(err));
			}
		}
	}
}

export template<> struct std::formatter<net::ErrorCodes>
{
	std::format_parse_context::iterator parse(std::format_parse_context& context) const noexcept
	{
		return context.begin();
	}

	auto format(const net::ErrorCodes& err, std::format_context& context) const noexcept
	{
		return std::format_to(context.out(), "{}", to_string(err));
	}
};
