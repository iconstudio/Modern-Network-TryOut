#pragma comment(lib, "Modern Network.lib")
#include <print>
#include <exception>
#include <coroutine>
#include <thread>
#include <future>

import Net.Constraints;
import Net.Application;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Task;

void Worker()
{
	net::Socket test_socket1 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	auto binded = test_socket1.Bind(net::IPv4Address::Loopback, 52001);
	if (binded.has_value())
	{
		std::println("binded! (1)");
	}

	net::Socket test_socket2 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	binded = test_socket2.Bind(net::IPv4Address::Loopback, 52002);
	if (binded.has_value())
	{
		std::println("binded! (2)");
	}

	auto connected = test_socket1.Connect(net::IPv4Address::Loopback, 52000);
	if (connected.has_value())
	{
		std::println("connected! (1)");
	}

	auto connector = test_socket2.ConnectAsync(net::IPv4Address::Loopback, 52000);
	connector();

	auto b = connector.Result();

	if (b.has_value())
	{
		std::println("connected! (2)");
	}

	std::println("Worker done!");
}

int main()
{
	std::println("=========== Init ===========");
	net::Application app{};

	std::println("=========== Awake ===========");
	app.Awake();

	std::println("=========== Start ===========");
	app.Start();

	Worker();

	std::println("=========== Update ===========");
	while (true)
	{
		app.Update(0);
	}
}
