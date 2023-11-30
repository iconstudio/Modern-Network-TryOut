module;
#define NOMINMAX
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

module Net.Io.Station;
import <thread>;

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
	auto ptr = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, std::move(concurrency_hint));
	NativeHandle io_port = NativeHandle::Create(ptr);

	if (not io_port)
	{
		return std::unexpected(AcquireNetworkError());
	}

	return Station{ std::move(io_port) };
}

net::io::Station::Station(net::NativeHandle&& handle)
noexcept
	: Handler(std::move(handle)), mySwitch()
{}

std::stop_token
net::io::Station::MakeCancelToken()
const noexcept
{
	return mySwitch.get_token();
}

net::SocketResult
net::io::Station::Register(net::Socket& socket, std::uint64_t id)
noexcept
{
#if _DEBUG
	auto& sk = socket.GetHandle();
	const HANDLE target = reinterpret_cast<::HANDLE>(sk);

	auto& handle = GetHandle();
	auto ptr = handle.GetPointer();

	auto port = ::CreateIoCompletionPort(target, const_cast<void*>(ptr), id, 0);
#else
	auto port = ::CreateIoCompletionPort(reinterpret_cast<::HANDLE>(socket.GetHandle()), const_cast<void*>(GetHandle().GetPointer()), id, 0);
#endif

	WSA_FLAG_OVERLAPPED;
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

bool
net::io::Station::Destroy()
noexcept
{
	if (auto& handle = GetHandle(); handle)
	{
		const bool result = (1 == handle.Delegate(::CloseHandle));
		myHandle = nullptr;

		return result;
	}
	else
	{
		return false;
	}
}

bool
net::io::Station::Destroy(net::ErrorCodes& error_code)
noexcept
{
	if (auto result = Destroy(); result)
	{
		return true;
	}
	else
	{
		error_code = AcquireNetworkError();
		return false;
	}
}

net::io::Station::Awaiter
net::io::Station::Schedule()
noexcept
{
	return Awaiter{ *this };
}

net::io::Station::Awaiter
net::io::Station::operator co_await()
noexcept
{
	return Awaiter{ *this };
}

std::unique_ptr<net::io::Schedule>
net::io::Station::Awaiter::await_resume()
{
	return std::make_unique<net::io::Schedule>(ioStation, ioStation.MakeCancelToken());
}
