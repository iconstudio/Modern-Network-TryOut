#pragma comment(lib, "Modern Network.lib")
#include <print>

import Net.Constraints;
import Net.Application;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Task;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Timed;

net::Coroutine Worker2()
{
	net::Socket test_socket2 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	auto binded = test_socket2.Bind(net::IPv4Address::Loopback, 52002);
	if (binded.has_value())
	{
		std::println("binded! (2)");
	}

	auto a = co_await net::coroutine::WaitForSeconds(3);

	//auto connector = test_socket2.ConnectAsync(net::IPv4Address::Loopback, 52000);
	//auto b = co_await connector;

	auto sdsc = co_await test_socket2.ConnectAsync(net::IPv4Address::Loopback, 52000);

	//if (b.has_value())
	if (sdsc.has_value())
	{
		std::println("connected! (2)");
	}

	co_return;
}

void Worker()
{
	net::Socket test_socket1 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	auto binded = test_socket1.Bind(net::IPv4Address::Loopback, 52001);
	if (binded.has_value())
	{
		std::println("binded! (1)");
	}

	auto connected = test_socket1.Connect(net::IPv4Address::Loopback, 52000);
	if (connected.has_value())
	{
		std::println("connected! (1)");
	}

	Worker2();

	std::println("Worker done!");
}

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::Application app{};

	std::println("=========== Awake ===========");
	app.Awake();

	std::println("=========== Start ===========");
	app.Start();

	Worker();

	std::println("=========== Update ===========");

	char buffer[512]{};
	while (true)
	{
		app.Update(0);
	}
}
