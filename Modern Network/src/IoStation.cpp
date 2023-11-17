module;
#define NOMINMAX
#include <WinSock2.h>

module Net.Io.Station;
import <memory>;
import <limits>;

net::io::Station::Stationary
net::io::Station::Create()
noexcept
{
	return Create(std::thread::hardware_concurrency());
}

net::io::Station::Stationary
net::io::Station::Create(std::uint32_t concurrency_hint)
noexcept
{
	NativeHandle io_port = NativeHandle::Create(::CreateIoCompletionPort(nullptr, nullptr, std::numeric_limits<ULONG_PTR>::max(), concurrency_hint));

	if (not io_port)
	{
		return std::unexpected(AcquireNetworkError());
	}

	return io_port;
}

constexpr
net::io::Station::Station(NativeHandle&& handle)
noexcept
	: Handler(std::move(handle))
{}
