﻿#pragma comment(lib, "Modern Network.lib")
#include <print>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.IoContext;
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
		net::SocketReceivingResult recv = client.Receive(std::span{ buffer });
		if (recv.has_value())
		{
			recv_size = recv.value();
			std::println("Server received {} bytes", recv_size);
		}
		else
		{
			std::println("Server receives are failed due to {}", recv.error());
			//break;
		}
		//auto sent = client.Send(buffer, recv_size);
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

	std::println("=========== Start ===========");

	if (listener.Open().has_value())
	{
		std::println("The listener is opened!");
	}

	auto acceptance = listener.Accept();
	client = std::move(acceptance.value());

	//client = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Update ===========");

	Worker();

	net::core::Annihilate();
}
