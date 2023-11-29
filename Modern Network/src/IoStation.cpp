module;
#define NOMINMAX
#include <WinSock2.h>

module Net.Io.Station;
import <thread>;
import <limits>;

net::SocketResult
net::io::Station::Register(net::Socket& socket, std::uint64_t id)
noexcept
{
	auto& target = socket.GetHandle();
	auto handle = GetHandle().GetPointer();
	auto port = ::CreateIoCompletionPort(reinterpret_cast<::HANDLE>(target), const_cast<void*>(handle), id, 0);

	NativeHandle result = NativeHandle::Create(port);

	if (not result)
	{
		return std::unexpected(AcquireNetworkError());
	}

	return 1;
}

bool
net::io::Station::TryRegister(net::Socket& socket, std::uint64_t id, net::ErrorCodes& error_code)
noexcept
{
	return false;
}

net::io::Station::Stationary
net::io::Station::Create()
noexcept
{
	return Create(std::numeric_limits<std::uint64_t>::max(), std::thread::hardware_concurrency());
}

net::io::Station::Stationary
net::io::Station::Create(std::uint32_t concurrency_hint)
noexcept
{
	return Create(std::numeric_limits<std::uint64_t>::max(), std::move(concurrency_hint));
}

net::io::Station::Stationary
net::io::Station::Create(std::uint64_t id) noexcept
{
	return Create(std::move(id), std::thread::hardware_concurrency());
}

net::io::Station::Stationary
net::io::Station::Create(std::uint64_t id, std::uint32_t concurrency_hint) noexcept
{
	NativeHandle io_port = NativeHandle::Create(::CreateIoCompletionPort(nullptr, nullptr, std::move(id), std::move(concurrency_hint)));

	if (not io_port)
	{
		return std::unexpected(AcquireNetworkError());
	}

	return Station{ std::move(io_port) };
}

constexpr
net::io::Station::Station(net::NativeHandle&& handle)
noexcept
	: Handler(std::move(handle))
{}
