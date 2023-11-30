module;
#define NOMINMAX
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>

module Net.Io.Schedule;
import Net.Io.Station;

net::io::Schedule::Awaiter
net::io::Schedule::Start()
{
	return { *this };
}

net::io::Event&
net::io::Schedule::Awaiter::await_resume()
noexcept
{
	auto& ev_handle = ioSchedule.GetEvent();

	::LPOVERLAPPED overlapped{};
	::BOOL result = ::GetQueuedCompletionStatus(ioSchedule.ioStation.GetHandle()
		, std::addressof(ev_handle.ioBytes)
		, std::addressof(ev_handle.eventId)
		, std::addressof(overlapped)
		, INFINITE);

	ev_handle.ioContext = reinterpret_cast<net::io::Context*>(overlapped);
	ev_handle.isSucceed = (1 == result);

	return ev_handle;
}
