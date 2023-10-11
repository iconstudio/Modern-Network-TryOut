#pragma comment(lib, "Modern Network.lib")

import <print>;
import Net.Application;

int main()
{
	net::Application app{};
	app.Awake();
	app.Start();
	app.Update(0);
}
