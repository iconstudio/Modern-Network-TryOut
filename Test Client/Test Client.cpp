#pragma comment(lib, "Modern Network.lib")
#include <print>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Timed;

net::Socket client = net::Socket::EmptySocket;

net::Coroutine Worker()
{
	co_await net::coroutine::WaitForSeconds(1);

	net::EndPoint server{ net::IPv4Address::Loopback, 52000 };

	if (auto connectum = co_await client.ConnectAsync(server); connectum.has_value())
	{
		std::println("The client is connected!");
	}
	else
	{
		std::println("The client cannot be connected!");
	}

	char buffer[512]{};
	while (true)
	{
	}
}

int main()
{
	std::println("=========$ Client $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");
	client = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Start ===========");

	if (client.Bind(net::IPv4Address::Loopback, 52001).has_value())
	{
		std::println("The client is binded!");
	}

	std::println("=========== Update ===========");
	Worker();

	net::core::Annihilate();
}
