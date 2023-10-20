#pragma comment(lib, "Modern Network.lib")
#include <print>

import Net.Constraints;
import Net.Application;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Task;
import Net.Coroutine;

int main()
{
	std::println("=========$ Client $=========");
	std::println("=========== Init ===========");
	net::Application app{};

	std::println("=========== Awake ===========");
	app.Awake();

	std::println("=========== Start ===========");
	app.Start();

	std::println("=========== Update ===========");

	char buffer[512]{};
	while (true)
	{
		app.Update(0);
	}
}
