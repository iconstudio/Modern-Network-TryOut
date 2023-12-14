#pragma comment(lib, "Modern Network.lib")
#include <cstdlib>
#include <array>
#include <span>
#include <print>
#include <thread>
#include <atomic>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Io.Context;
import Net.Io.Station;
import Net.Socket;
import Net.SocketPool;

void Worker(size_t nth);

enum class IoOperation
{
	None = 0, Accept, Connect, Recv, Send, Close,
	BroadcastMessage
};

class ExContext : public net::io::Context
{
public:
	using Context::Context;

	size_t myID;
	IoOperation myOperation;
};

class ChatMsgContext : public ExContext
{
public:
	using ExContext::ExContext;

	std::atomic_int refCount;
};

class Client
{
public:
	net::Socket* mySocket;
	ExContext myContext;
};

net::Socket serverListener{};
static inline constexpr size_t clientsNumber = 40;
static inline constexpr size_t sizeRecvBuffer = 256;
static inline constexpr size_t clientIdOffset = 1;

net::SocketPool everySockets{ clientsNumber };
std::array<Client*, clientsNumber> everyClients{};

std::array<ExContext*, clientsNumber> clientContexts{};
// all-in-one circular buffer
std::array<std::byte, clientsNumber* sizeRecvBuffer> clientsRecvBuffer{};

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

	everySockets.Add(&serverListener, 0ULL);
	std::println("The listener is ready.");

	for (auto& ctx : clientContexts)
	{
		ctx = new ExContext{};
	}
	std::println("Contexts are ready.");

	for (size_t i = 0; i < clientsNumber; ++i)
	{
		const size_t id = i + clientIdOffset;

		auto socket = everySockets.Allocate(id, net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

		socket->IsAddressReusable = true;

		auto& ctx = clientContexts[i];
		ctx->myID = id;
		ctx->myOperation = IoOperation::None;
	}
	std::println("Clients are ready.");

	lastClient = everySockets.begin();

	std::println("=========== Start ===========");
	if (serverListener.Open().has_value())
	{
		std::println("The listener is opened.");
	}

	std::vector<std::jthread> workers{};
	for (size_t i = 0; i < 6; ++i)
	{
		auto& worker = workers.emplace_back(Worker, i);
	}

	for (auto& client : everySockets)
	{
		auto& id = client.id;
		auto& socket = *client.sk;

		if (0 == id)
		{
			continue;
		}

		auto& ctx = clientContexts[GetIndexOnID(id)];
		ctx->myOperation = IoOperation::Accept;

		auto acceptance = serverListener.ReserveAccept(*ctx, socket);
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

void Worker(size_t nth)
{
	//auto io_schedule = everySockets.Schedule();
	std::println("Worker {} is started", nth);

	while (true)
	{
		//if (io_schedule.IsCancelled())
		{
			//std::println("Worker has been cancelled");
			//break;
		}

		auto io_event = everySockets.WaitForIoResult();
		auto& io_context = io_event.ioContext;
		auto& io_id = io_event.eventId;

		auto ex_context = static_cast<ExContext*>(io_context);
		if (ex_context)
		{
			ex_context->Clear();

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

					op = IoOperation::Recv;
					std::println("Client connected - {}", id);

					auto it = everySockets.Find(id);
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

						auto it = everySockets.Find(id);
						auto& client = *it;
						auto& socket = client.sk;

						op = IoOperation::Close;
						socket->CloseAsync(ex_context);

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

							auto it = everySockets.Find(id);
							auto& client = *it;
							auto& socket = client.sk;

							auto& ctx = clientContexts[GetIndexOnID(id)];
							ctx->myOperation = IoOperation::Close;

							socket->CloseAsync(ctx);
						}

						break; // switch (op)
					}

					delete ex_context;
				}
				break;

				case IoOperation::BroadcastMessage:
				{

				}
				break;

				case IoOperation::Close:
				{
					ex_context->Clear();
					std::println("Client {} is closed", id);

					// accept again
					op = IoOperation::Accept;
					auto it = everySockets.Find(id);
					auto& client = *it;
					auto& socket = client.sk;

					auto acceptance = serverListener.ReserveAccept(ex_context, *socket);
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
