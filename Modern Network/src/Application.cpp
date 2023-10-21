module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <MSWSock.h>
#include <minwindef.h>
#include <type_traits>
#include <utility>
#include <format>
#include <print>

module Net.Application;
import Net;
import Net.Socket;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Exception.NetworkInitializationError;
import Net.Exception.ServerSetupError;

using namespace net;

Socket serverSocket = Socket::EmptySocket;

net::Application::Application()
{
	auto error = core::Initialize();
	if (error)
	{
		NetworkInitializationError startup_error;

		constexpr auto startup_notify_msg = "Error on Network Initialization (Code: {})";
		auto formatted_msg = std::format(startup_notify_msg, static_cast<unsigned int>(error.value()));
		startup_error = NetworkInitializationError{ formatted_msg.c_str() };

		throw startup_error;
	}
}

net::Application::~Application() noexcept
{
	core::Annihilate();
}

void
net::Application::Awake()
{
	serverSocket = Socket::Create(InternetProtocols::TCP, IpAddressFamily::IPv4);

	serverSocket.Bind(IPv4Address::Loopback, 52000).or_else(
		[](ErrorCodes&& error_code) -> std::expected<unsigned int, ErrorCodes> {
		constexpr auto startup_notify_msg = "Cannot bind the address to the listener (Code: {})";
		auto formatted_msg = std::format(startup_notify_msg, static_cast<int>(error_code));

		throw ServerSetupError{ formatted_msg.c_str() };
	});

	constexpr DWORD rio_control = SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER;
	RIO_EXTENSION_FUNCTION_TABLE rio_table{};
	GUID rio_fn_id = WSAID_MULTIPLE_RIO;
	DWORD _ = 0;

	if (0 != WSAIoctl(serverSocket.GetHandle()
		, rio_control
		, std::addressof(rio_fn_id)
		, sizeof(GUID)
		, (void**)std::addressof(rio_table)
		, sizeof(rio_table)
		, std::addressof(_)
		, nullptr, nullptr))
	{
		constexpr auto startup_notify_msg = "Cannot bind the address to the listener (Code: {})";
		auto formatted_msg = std::format(startup_notify_msg, ::WSAGetLastError());

		throw ServerSetupError{ formatted_msg.c_str() };
	}

	serverSocket.IsAddressReusable = true;
}

void
net::Application::Start()
{
	serverSocket.Open();
}

void
net::Application::Update(float)
{}

net::Socket&
net::Application::Listener()
noexcept
{
	return serverSocket;
}
