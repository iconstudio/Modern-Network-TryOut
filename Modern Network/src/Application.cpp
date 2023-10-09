module;
#include <WinSock2.h>
#include <MSWSock.h>
#include <minwindef.h>

module Net.Application;
import <type_traits>;
import <utility>;

using namespace net;

constexpr
net::Application::Application() noexcept
{}

net::Application::~Application() noexcept
{}

void
net::Application::Awake()
{
	WSADATA version_data{};

	const int startup = ::WSAStartup(MAKEWORD(2, 2), std::addressof(version_data));
	if (0 != startup)
	{
		throw;
	}

	RIO_EXTENSION_FUNCTION_TABLE rio_table{};
}

void
net::Application::Start()
{}

void
net::Application::Update(float)
{}
