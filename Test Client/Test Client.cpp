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

net::Socket client = net::Socket::EmptySocket;

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

net::Coroutine Worker()
{
	//co_await net::coroutine::WaitForSeconds(1);

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
		const int scan = scanf_s("%s", buffer, static_cast<unsigned>(sizeof(buffer)));

		if (EOF != scan)
		{
			auto sent = client.Send(buffer, as_string(buffer).size());
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

	client.IsAddressReusable = true;

	std::println("=========== Update ===========");
	Worker().Start();

	while (true)
	{}

	net::core::Annihilate();
}
