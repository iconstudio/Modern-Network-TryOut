#pragma comment(lib, "Modern Network.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <memory>
#include <string_view>
#include <print>
#include <array>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Socket;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.Awaiter.Concurrent;
//import Net.Scheduler;

net::Socket serverListener = net::Socket::EmptySocket;
//net::coroutine::Scheduler globalScheduler{};

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

net::Coroutine Accepter()
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

	co_return;
}

net::Coroutine Runner()
{
	std::println("Accepter started");
	co_await Accepter();

	std::println("Worker started");

	co_await net::coroutine::WaitForSeconds(1);

	net::io::Context listen_context{};
	listen_context.Clear();

	std::byte recv_buffer[512]{};
	size_t recv_size = 0;
	std::memset(recv_buffer, 0, sizeof(recv_buffer));

	//net::SocketClosingErrorCodes close_err;
	//const bool closed = lastClient.Close(close_err);

	std::span buffer{ recv_buffer };
	std::span<std::byte> send_buf;

	while (true)
	{
		net::SocketReceivingResult recv = lastClient.Receive(listen_context, buffer);
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

	co_return;
}

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");

	serverListener = net::Socket::Create(net::SocketType::Synchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	if (serverListener.BindHost(10000))
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

	Runner()();

	while (true)
	{

	}

	net::core::Annihilate();
}
