#pragma comment(lib, "Modern Network.lib")
#include <memory>
#include <string_view>
#include <print>
#include <array>
#include <span>
#include <vector>

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
net::io::Context acceptContext{};

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
	std::println("Accepter started");

	lastClient = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	auto reg_result = ioStation.Register(lastClient, 0);
	if (reg_result)
	{
		std::println("The client is registered");
	}
	else
	{
		std::println("The client is not registered");
	}

	//while (true)
	{
		//auto acceptance = serverListener.Accept();

		auto acceptance2 = serverListener.ReserveAccept(acceptContext, lastClient);
		auto&& io_schedule = co_await ioStation.Schedule();

		//if (acceptance.has_value())
		if (acceptance2.has_value())
		{
			acceptContext.Clear();
			//lastClient = std::move(acceptance2.value());
		}
		else
		{
			//std::println("The acceptance is failed due to '{}'", acceptance.error());
			std::println("The acceptance is failed due to '{}'", acceptance2.error());
			//break;
		}
	}

	co_await net::coroutine::WaitForSeconds(1);
	co_return;
}

net::Coroutine Worker()
{
	std::println("Worker is started");

	auto&& io_schedule = co_await ioStation.Schedule();

	while (true)
	{
		if (io_schedule->IsCancelled())
		{
			std::println("Worker has been cancelled");
			break;
		}

		auto& io_event = co_await io_schedule->Start();

		if (not io_event.isSucceed)
		{
			std::println("Worker has been failed");
			break;
		}
	}
}

net::Coroutine Receiver()
{
	std::println("Receiver is started");

	net::io::Context listen_context{};
	listen_context.Clear();

	std::byte recv_buffer[512]{};
	size_t recv_size = 0;
	//net::SocketClosingErrorCodes close_err;
	//const bool closed = lastClient.Close(close_err);

	std::span buffer{ recv_buffer };
	std::span<std::byte> send_buf;

	std::println("Receiving is started");

	while (true)
	{
		auto recv_task = lastClient.MakeReceiveTask(listen_context, buffer);

		auto recv = co_await recv_task;

		if (recv.has_value())
		{
			recv_size = recv.value();
			send_buf = buffer.subspan(0, recv_size);

			std::println("Server received '{}' bytes: {}", recv_size, as_string(send_buf, recv_size));
			listen_context.Clear();

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

	auto station = net::io::Station::Create();
	ioStation = std::move(station.value());
	if (ioStation.GetHandle())
	{
		std::println("The station is created");
	}
	else
	{
		std::println("The station has failed to be created");
	}

	serverListener = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	const net::EndPoint server{ net::IPv4Address::Loopback, 10000 };

	//if (serverListener.BindHost(10000))
	if (serverListener.Bind(server))
	{
		std::println("The listener is binded!");
	}

	serverListener.IsAddressReusable = true;

	auto reg_result = ioStation.Register(serverListener, 1);
	if (reg_result)
	{
		std::println("The listener is linked to the port");
	}
	else
	{
		std::println("The listener is not linked to the port");
	}

	std::println("=========== Start ===========");

	if (serverListener.Open().has_value())
	{
		std::println("The listener is opened!");
	}

	std::println("=========== Update ===========");

	auto runner = Accepter();
	runner.Start();
	auto worker = Receiver();
	worker.StartAsync();

	std::vector<net::Coroutine> workers{};
	for (size_t i = 0; i < 6; ++i)
	{
		auto& worker = workers.emplace_back(Worker());
		worker.StartAsync();
	}

	//std::println("Server is ended");

	while (true)
	{

	}

	net::core::Annihilate();
}
