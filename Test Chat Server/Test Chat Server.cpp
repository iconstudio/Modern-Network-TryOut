#pragma comment(lib, "Modern Network.lib")
#include <print>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Io.Station;
import Net.Socket;
import Net.SocketPool;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Concurrent;

net::Socket serverListener{};

constexpr size_t clientsNumber = 40;
net::SocketPool clientPool{ clientsNumber };

net::Coroutine Worker();

enum class IoOperation
{
	None = 0, Accept, Connect, Recv, Send, Close,
};

class ExContext : public net::io::Context
{
public:
	using Context::Context;

	IoOperation myOperation;
};

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");
	serverListener = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	serverListener.Bind(net::EndPoint{ net::IPv4Address::Loopback, 10000 });
	serverListener.IsAddressReusable = true;

	clientPool.Add(&serverListener, 0ULL);
	for (size_t i = 0; i < clientsNumber; ++i)
	{
		auto socket = clientPool.Allocate(i + 1, net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

		socket->IsAddressReusable = true;
	}

	std::println("=========== Start ===========");
	std::vector<net::Coroutine> workers{};
	for (size_t i = 0; i < 6; ++i)
	{
		auto& worker = workers.emplace_back(Worker());
		worker.StartAsync();
	}

	std::println("=========== Update ===========");

	while (true)
	{

	}
	net::core::Annihilate();
}

net::Coroutine Worker()
{

}
