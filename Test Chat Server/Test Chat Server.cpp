#pragma comment(lib, "Modern Network.lib")
#include <cstdlib>
#include <array>
#include <span>
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

net::Coroutine Worker(size_t nth);

enum class IoOperation
{
	None = 0, Accept, Connect, Recv, Send, Close,
};

class ExContext : public net::io::Context
{
public:
	using Context::Context;

	size_t myID;
	IoOperation myOperation;
};

net::Socket serverListener{};
static inline constexpr size_t clientsNumber = 40;
static inline constexpr size_t sizeRecvBuffer = 256;
static inline constexpr size_t clientIdOffset = 1;
net::SocketPool clientPool{ clientsNumber };
std::array<ExContext, clientsNumber> clientContexts{};
// all-in-one circular buffer
std::array<std::byte, clientsNumber * sizeRecvBuffer> clientsRecvBuffer{};

[[nodiscard]]
constexpr size_t GetIndexOnID(const size_t& id) noexcept
{
	return id - clientIdOffset;
}

[[nodiscard]]
constexpr std::span<std::byte, sizeRecvBuffer>
GetBuffer(const size_t& id)
noexcept
{
	return std::span<std::byte, sizeRecvBuffer>(clientsRecvBuffer.data() + GetIndexOnID(id) * sizeRecvBuffer, sizeRecvBuffer);
}

net::SocketPool::iterator lastClient{};

int main()
{
	std::println("=========$ Server $=========");
	std::println("=========== Init ===========");
	net::core::Initialize();

	std::println("=========== Awake ===========");
	serverListener = net::Socket::Create(net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	serverListener.Bind(net::EndPoint{ net::IPv4Address::Loopback, 10000 });
	serverListener.IsAddressReusable = true;

	clientPool.Add(&serverListener, 0ULL);
	std::println("The listener is ready.");

	for (size_t i = 0; i < clientsNumber; ++i)
	{
		const size_t id = i + clientIdOffset;

		auto socket = clientPool.Allocate(id, net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

		socket->IsAddressReusable = true;

		auto& ctx = clientContexts[i];
		ctx.myID = id;
		ctx.myOperation = IoOperation::None;
	}
	std::println("Clients are ready.");

	lastClient = clientPool.begin();

	std::println("=========== Start ===========");
	if (serverListener.Open().has_value())
	{
		std::println("The listener is opened.");
	}

	std::vector<net::Coroutine> workers{};
	for (size_t i = 0; i < 6; ++i)
	{
		auto& worker = workers.emplace_back(Worker(i));
		worker.StartAsync();
	}

	for (auto& client : clientPool)
	{
		auto& id = client.id;
		auto& socket = *client.sk;

		if (0 == id)
		{
			continue;
		}

		auto& ctx = clientContexts[GetIndexOnID(id)];
		ctx.myOperation = IoOperation::Accept;

		auto acceptance = serverListener.ReserveAccept(ctx, socket);
		if (not acceptance)
		{
			std::abort();
			break;
		}
	}
	std::println("Clients are reverved to accept.");

	std::println("=========== Update ===========");

	while (true)
	{

	}
	net::core::Annihilate();
}

net::Coroutine Worker(size_t nth)
{
	auto&& io_schedule = co_await clientPool.Schedule();
	std::println("Worker {} is started", nth);

	while (true)
	{
		if (io_schedule->IsCancelled())
		{
			std::println("Worker has been cancelled");
			break;
		}

		auto& io_event = co_await io_schedule->Start();
		auto& io_context = io_event.ioContext;
		auto& io_id = io_event.eventId;

		auto ex_context = static_cast<ExContext*>(io_context);
		if (ex_context)
		{
			ex_context->Clear();

			if (0 <= io_id)
			//if (clientIdOffset <= io_id)
			{
				auto& op = ex_context->myOperation;
				const auto& id = ex_context->myID;

				switch (op)
				{
					case IoOperation::Accept:
					{
						if (not io_event.isSucceed)
						{
							std::println("Worker has been failed as acceptance on {}", id);
							break; // switch (op)
						}

						ex_context->myOperation = IoOperation::Recv;
						std::println("Client connected - {}", id);

						auto it = clientPool.Find(id);
						auto& client = *it;
						auto& socket = client.sk;

						auto r = socket->Receive(*ex_context, GetBuffer(id));

						if (r)
						{
							std::println("Client {}'s receive are reserved", id);
						}
						else
						{
							std::println("Client {}'s receive are not able to be reserved ({})", id, r.error());
						}
					}
					break;

					case IoOperation::Recv:
					{
						if (not io_event.isSucceed)
						{
							std::println("Worker has been failed as receiving on client {}", id);

							auto it = clientPool.Find(id);
							auto& client = *it;
							auto& socket = client.sk;

							ex_context->myOperation = IoOperation::Close;
							socket->CloseAsync(*ex_context);

							break; // switch (op)
						}

						auto& bytes = io_event.ioBytes;
					}
					break;

					case IoOperation::Send:
					{
						const auto& bytes = io_event.ioBytes;

						if (not io_event.isSucceed)
						{
							std::println("Worker has been failed as sending on client {}", id);

							delete ex_context;

							if (0 == bytes)
							{
								std::println("Closing client {} as sending has been failed", id);

								auto it = clientPool.Find(id);
								auto& client = *it;
								auto& socket = client.sk;

								auto& ctx = clientContexts[GetIndexOnID(id)];
								ctx.myOperation = IoOperation::Close;

								socket->CloseAsync(ctx);
							}

							break; // switch (op)
						}

						delete ex_context;
					}
					break;

					case IoOperation::Close:
					{
						ex_context->Clear();
						std::println("Client {} is closed", id);

						// accept again
						ex_context->myOperation = IoOperation::Accept;
						auto it = clientPool.Find(id);
						auto& client = *it;
						auto& socket = client.sk;

						auto acceptance = serverListener.ReserveAccept(*ex_context, *socket);
						if (not acceptance)
						{
							std::println("Client {} cannot be accepted due to {}", id, acceptance.error());

							std::abort();
							break;
						}
					}
					break;

					default:
					{
						std::println("Unknown task on {}", id);
					}
				}
			}
		}
	}
}
