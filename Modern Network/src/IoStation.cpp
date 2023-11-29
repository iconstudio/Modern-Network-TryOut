module;
#define NOMINMAX
#include <WinSock2.h>

module Net.Io.Station;
import <thread>;

net::io::Station::~Station()
noexcept
{
	::CloseHandle(std::move(*this).GetHandle().GetPointer());
}

net::SocketResult
net::io::Station::Register(net::Socket& socket, std::uint64_t id)
const noexcept
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
const noexcept
{
	if (auto result = Register(socket, id); result)
	{
		return true;
	}
	else
	{
		error_code = std::move(result).error();
		return false;
	}
}

net::io::Station::Stationary
net::io::Station::Create()
noexcept
{
	return Create(defaultID, std::thread::hardware_concurrency());
}

net::io::Station::Stationary
net::io::Station::Create(std::uint32_t concurrency_hint)
noexcept
{
	return Create(defaultID, std::move(concurrency_hint));
}

net::io::Station::Stationary
net::io::Station::Create(std::uint64_t id) noexcept
{
	return Create(std::move(id), std::thread::hardware_concurrency());
}

net::io::Station::Stationary
net::io::Station::Create(std::uint64_t id, std::uint32_t concurrency_hint) noexcept
{
	auto ptr = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, std::move(id), std::move(concurrency_hint));
	NativeHandle io_port = NativeHandle::Create(ptr);

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
