export module Net.Socket:Errors;
import :ErrorCode;
import <expected>;

export namespace net
{
	using SocketResult = std::expected<unsigned int, SocketErrorCode>;
}
