export module Net.Socket:Errors;
import :ErrorCode;
import <optional>;

export namespace net
{
	using SocketError = std::optional<SocketErrorCode>;
}
