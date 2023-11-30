module;
#define NOMINMAX
#include <WinSock2.h>

module Net.Io.Station;
import <thread>;

net::io::Station::~Station()
noexcept
{
	if (auto handle = std::move(*this).GetHandle().GetPointer(); handle)
	{
		::CloseHandle(handle);
	}
}

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
