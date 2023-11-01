#pragma comment(lib, "Modern Network.lib")
#include <string_view>
#include <print>
#include <span>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Socket;
import Net.Coroutine;
import Net.Coroutine.Awaiter.Timed;
import Net.Coroutine.Awaiter.Concurrent;

net::Socket listener = net::Socket::EmptySocket;
net::Socket client = net::Socket::EmptySocket;

[[nodiscard]]
inline std::string_view as_string(const std::span<const std::byte> buffer) noexcept
{
	return std::string_view{ reinterpret_cast<const char*>(buffer.data()) };
}

[[nodiscard]]
inline std::string_view as_string(const std::span<const std::byte> buffer, const size_t size) noexcept
{
	return std::string_view{ reinterpret_cast<const char*>(buffer.data()), size };
}

net::Coroutine Worker()
{
	co_await net::coroutine::WaitForSeconds(1);

	net::io::Context listen_context{};
	std::memset(&listen_context, 0, sizeof(listen_context));

	std::byte recv_buffer[512]{};
	size_t recv_size = 0;
	std::memset(recv_buffer, 0, sizeof(recv_buffer));

	//net::SocketClosingErrorCodes close_err;
	//const bool closed = client.Close(close_err);

	std::span buffer{ recv_buffer };
	std::span<std::byte> send_buf;

	while (true)
	{
		net::SocketReceivingResult recv = client.Receive(listen_context, buffer);
		listen_context.Clear();

		if (recv.has_value())
		{
			recv_size = recv.value();
			send_buf = buffer.subspan(0, recv_size);

			std::println("Server received '{}' bytes: {}", recv_size, as_string(send_buf, recv_size));
		}
		else
		{
			std::println("Server receives are failed due to '{}'", recv.error());
			break;
		}

		auto sent = client.Send(listen_context, send_buf, recv_size);
		listen_context.Clear();
	}

	co_return;
}

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");

	listener = net::Socket::Create(net::SocketType::Synchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	//if (listener.Bind(net::IPv4Address::Loopback, 52000).has_value())
	if (listener.BindHost(10000))
	{
		std::println("The listener is binded!");
	}

	listener.IsAddressReusable = true;

	std::println("=========== Start ===========");

	if (listener.Open().has_value())
	{
		std::println("The listener is opened!");
	}

	auto acceptance = listener.Accept();
	client = std::move(acceptance.value());

	std::println("=========== Update ===========");

	Worker();

	net::core::Annihilate();
}
