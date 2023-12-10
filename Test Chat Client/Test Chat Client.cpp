#pragma comment(lib, "Modern Network.lib")
#include <print>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Io.Station;
import Net.Socket;
import Net.SocketPool;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Concurrent;

net::Socket mySocket{};

net::Coroutine Accepter();
net::Coroutine Worker();

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");
	serverListener = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	std::println("=========== Start ===========");

}
