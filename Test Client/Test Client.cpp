#pragma comment(lib, "Modern Network.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <cstdlib>
#include <cstddef>
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

	net::EndPoint server{ net::IPv4Address::Loopback, 10000 };

	if (client.Connect(server).has_value())
	{
		std::println("The client is connected!");
	}
	else
	{
		std::println("The client cannot be connected!");
	}

	//if (auto connectum = co_await client.ConnectAsync(server); connectum.has_value())

	std::byte buffer[512]{};
	while (true)
	{
		const int cnt = scanf_s("%s", buffer, static_cast<unsigned>(sizeof(buffer)));

		if (0 < cnt)
		{
			auto sent = client.Send(buffer);
			std::println("The client sent: {}", reinterpret_cast<const char*>(buffer));
		}
	}

	co_return;
}

int main()
{
	std::println("=========$ Client $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");
	client = net::Socket::Create(net::SocketType::Synchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Start ===========");

	if (client.Bind(net::IPv4Address::Loopback, 52001).has_value())
	{
		std::println("The client is binded!");
	}

	std::println("=========== Update ===========");
	Worker();

	net::core::Annihilate();
}
