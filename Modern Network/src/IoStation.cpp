module;
#define NOMINMAX
#include <WinSock2.h>

module Net.Io.Station;
import <memory>;
import <limits>;

net::io::Station
net::io::Station::Create()
noexcept
{
	Station result{};

	NativeHandle io_port = ::CreateIoCompletionPort(nullptr, nullptr, std::numeric_limits<ULONG_PTR>::max(), std::thread::hardware_concurrency());

	return result;
}
