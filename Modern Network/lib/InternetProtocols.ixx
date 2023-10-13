export module Net.InternetProtocols;
import <cstdint>;

export namespace net
{
	enum class [[nodiscard]] InternetProtocols : std::int32_t
	{
		Unknown, TCP, UDP
	};
}
