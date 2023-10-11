module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <minwindef.h>

module Net.Application;
import Net.Socket;
import Net.Exception.NetworkInitializationError;
import <type_traits>;
import <utility>;
import <format>;

using namespace net;

Socket serverSocket = Socket::EmptySocket;

net::Application::Application() noexcept
{}

net::Application::~Application() noexcept
{}

void
net::Application::Awake()
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

	RIO_EXTENSION_FUNCTION_TABLE rio_table{};

	serverSocket = Socket::Create();
}

void
net::Application::Start()
{}

void
net::Application::Update(float)
{}
