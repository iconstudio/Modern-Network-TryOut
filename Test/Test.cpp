﻿#pragma comment(lib, "Modern Network.lib")
#include <memory>
#include <string_view>
#include <print>
#include <array>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Io.Station;
import Net.Socket;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Concurrent;
//import Net.Scheduler;

net::Socket serverListener = net::Socket::EmptySocket;
//net::coroutine::Scheduler globalScheduler{};

net::io::Station ioStation;

constexpr size_t maxClients = 1000;
std::array<net::Socket*, maxClients> everyClients{};

size_t lastClientIndex = 0;
size_t clientsNumber = 0;
net::Socket lastClient = net::Socket::EmptySocket;

[[nodiscard]]
inline std::string_view as_string(const std::span<const std::byte> buffer) noexcept
{
	return std::string_view{ reinterpret_cast<const char*>(buffer.data()) };
}

[[nodiscard]]
inline std::string_view as_string(const std::span<const std::byte> buffer, const size_t size) noexcept
{
	return std::string_view{ reinterpret_cast<const char*>(buffer.data()), size };
}

void Accepter()
{
	//while (true)
	{
		auto acceptance = serverListener.Accept();

		if (acceptance.has_value())
		{
			lastClient = std::move(acceptance.value());
		}
		else
		{
			std::println("The acceptance is failed due to '{}'", acceptance.error());
			//break;
		}
	}
}

net::Coroutine Runner()
{
	auto station = net::io::Station::Create();
	ioStation = std::move(station.value());

	std::println("Accepter started");
	Accepter();

	std::println("Worker started");

	co_await net::coroutine::WaitForSeconds(1);

	net::io::Context listen_context{};
	listen_context.Clear();

	std::byte recv_buffer[512]{};
	size_t recv_size = 0;
	//std::memset(recv_buffer, 0, sizeof(recv_buffer));

	//net::SocketClosingErrorCodes close_err;
	//const bool closed = lastClient.Close(close_err);

	std::span buffer{ recv_buffer };
	std::span<std::byte> send_buf;

	while (true)
	{
		auto recv_task = lastClient.MakeReceiveTask(listen_context, buffer);
		//auto recv = co_await recv_task;
		auto recv = co_await recv_task;
		//auto recv = recv_task();

		listen_context.Clear();

		if (recv.has_value())
		{
			recv_size = recv.value();
			send_buf = buffer.subspan(0, recv_size);

			std::println("Server received '{}' bytes: {}", recv_size, as_string(send_buf, recv_size));

			auto sent = lastClient.Send(listen_context, send_buf, recv_size);
			listen_context.Clear();
		}
		else
		{
			std::println("Server receives are failed due to '{}'", recv.error());
			break;
		}
	}
}

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");

	serverListener = net::Socket::Create(net::SocketType::Synchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	const net::EndPoint server{ net::IPv4Address::Loopback, 10000 };

	//if (serverListener.BindHost(10000))
	if (serverListener.Bind(server))
	{
		std::println("The listener is binded!");
	}

	serverListener.IsAddressReusable = true;

	std::println("=========== Start ===========");

	if (serverListener.Open().has_value())
	{
		std::println("The listener is opened!");
	}

	std::println("=========== Update ===========");

	Runner();

	while (true)
	{

	}

	net::core::Annihilate();
}
