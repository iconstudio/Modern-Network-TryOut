module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <minwindef.h>

module Net.Application;
import Net.Socket;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Exception.NetworkInitializationError;
import <type_traits>;
import <utility>;
import <format>;
import <print>;

using namespace net;

Socket serverSocket = Socket::EmptySocket;

net::Application::Application()
{
	WSADATA version_data{};

	NetworkInitializationError startup_error;

	const int startup = ::WSAStartup(MAKEWORD(2, 2), std::addressof(version_data));
	if (0 != startup)
	{
		constexpr auto startup_notify_msg = "Error on Network Initialization (Code: {})";
		auto formatted_msg = std::format(startup_notify_msg, startup);
		startup_error = NetworkInitializationError{ formatted_msg.c_str() };

		throw startup_error;
	}
}

net::Application::~Application() noexcept
{
	::WSACleanup();
}

void
net::Application::Awake()
{
	RIO_EXTENSION_FUNCTION_TABLE rio_table{};

	serverSocket = Socket::Create(InternetProtocols::TCP, IpAddressFamily::IPv4);

	auto binded = serverSocket.Bind(IPv4Address::Loopback, 52000);

	if (binded.has_value())
	{
		std::println("binded!");
	}
}

void
net::Application::Start()
{}

void
net::Application::Update(float)
{}
