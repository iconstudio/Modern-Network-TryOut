#pragma comment(lib, "Modern Network.lib")
import <cstdio>;
import <print>;

import Test.Framework;

test::Framework serverFramework{};

int main()
{
	std::println("=========$ Server $=========");
	test::ServerPreset my_setup
	{
		.serverID = 0,
		.serverAddress = net::EndPoint{ net::IPv4Address::Loopback, 10000 }
	};

	std::println("=========== Init ===========");
	serverFramework.Awake(std::move(my_setup));

	std::println("=========== Start ===========");
	serverFramework.Start(6);

	std::println("=========== Update ===========");

	while (true)
	{
		char command[256]{};

		auto input = ::scanf_s("%s", command, sizeof(command));
		if (EOF != input)
		{
			if (command[0] == 'q')
			{
				break;
			}
		}
	}

	std::println("========== Destroy ===========");
	serverFramework.Destroy();

	std::println("========== Clean up ==========");
	serverFramework.Cleanup();
}
