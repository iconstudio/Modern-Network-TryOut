#pragma comment(lib, "Modern Network.lib")

import <print>;
import Net.Application;

int main()
{
	std::println("Init");
	net::Application app{};

	std::println("Awake");
	app.Awake();

	std::println("Start");
	app.Start();

	std::println("Update");
	app.Update(0);
}
