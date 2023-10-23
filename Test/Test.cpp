#pragma comment(lib, "Modern Network.lib")
#include <print>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.Awaiter.Concurrent;

net::Socket listener = net::Socket::EmptySocket;
net::Socket client = net::Socket::EmptySocket;

net::Coroutine Worker()
{
	co_await net::coroutine::WaitForSeconds(1);

	net::IoContext listen_context{};
	std::byte buffer[512]{};
	size_t recv_size = 0;

	while (true)
	{
		auto recv = co_await listener.ReceiveAsync(&listen_context, std::span{ buffer });
		if (recv.has_value())
		{
			recv_size = recv.value();
		}
		else
		{
			break;
		}
		auto sent = co_await client.SendAsync(&listen_context, buffer, recv_size);
	}

	co_return;
}

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");

	listener = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	if (listener.Bind(net::IPv4Address::Loopback, 52000).has_value())
	{
		std::println("The listener is binded!");
	}

	listener.IsAddressReusable = true;

	client = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Start ===========");

	if (listener.Open().has_value())
	{
		std::println("The listener is opened!");
	}

	std::println("=========== Update ===========");

	Worker();

	net::core::Annihilate();
}
