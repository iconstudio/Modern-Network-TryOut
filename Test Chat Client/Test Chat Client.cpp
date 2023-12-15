#pragma comment(lib, "Modern Network.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <cstdlib>
#include <cstddef>
#include <string_view>
#include <print>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Coroutine;
import Net.Io.Context;

net::Socket client = net::Socket::EmptySocket;
const net::EndPoint server_address{ net::IPv4Address::Loopback, 10000 };

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

net::io::Context sendContext{};
net::io::Context recvContext{};
std::byte recvBuffer[256]{};

net::CoAction Worker()
{
	//if (auto connectum = co_await client.ConnectAsync(server_address); connectum.has_value())

	std::byte buffer[512]{};
	while (true)
	{
		const int scan = scanf_s("%s", buffer, static_cast<unsigned>(sizeof(buffer)));

		if (EOF != scan)
		{
			auto sent = client.MakeSendTask(sendContext, buffer, as_string(buffer).size());

			auto sr = co_await sent;
			if (sr)
			{
				std::println("The client sent: {}", reinterpret_cast<const char*>(buffer));
				sendContext.Clear();
			}
			else
			{
				std::println("The client cannot send due to {}", sr.error());
				break; // while (true)
			}
		}
	}

	co_return;
}

net::CoAction Presenter()
{
	co_await net::coroutine::WaitForSeconds(1);

	while (true)
	{
		auto recv = client.MakeReceiveTask(recvContext, recvBuffer);

		auto rr = co_await recv;
		if (rr)
		{
			std::println("The client received: {}", as_string(recvBuffer));
			recvContext.Clear();
		}
		else
		{
			std::println("The client cannot receive due to {}", rr.error());
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
	client = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Start ===========");

	if (client.Bind(net::IPv4Address::Loopback, 52001).has_value())
	{
		std::println("The client is binded!");
	}

	client.IsAddressReusable = true;

	if (client.Connect(server_address).has_value())
	{
		std::println("The client is connected!");
	}
	else
	{
		std::println("The client cannot be connected!");
		return 1;
	}

	std::println("=========== Update ===========");
	Worker();
	Presenter();

	while (true)
	{}

	net::core::Annihilate();

	return 0;
}
