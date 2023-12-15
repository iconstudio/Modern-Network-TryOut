#pragma comment(lib, "Modern Network.lib")
#include <cstdlib>
#include <array>
#include <vector>
#include <span>
#include <print>
#include <string>
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

	std::uintptr_t myID;
	IoOperation myOperation;
};

class ChatMsgContext : public ExContext
{
public:
	using ExContext::ExContext;

	std::atomic_int refCount;
	std::string chatMsg;
};

class Client
{
public:
	net::Socket* mySocket;
	ExContext myContext;
};

enum class PacketCategory : unsigned char
{
	None = 0, Chat, SignIn, SignOut
};

#pragma pack(push, 1)
struct alignas(1) BasicPacket
{
	PacketCategory myCat;
	short mySize;
};

struct alignas(1) PointerPacket : public BasicPacket
{
	void* myData;

	[[nodiscard]]
	std::vector<std::byte> Serialize() const
	{
		return {};
	}
};
#pragma pack(pop)

net::Socket serverListener{};
static inline constexpr size_t clientsNumber = 40;
static inline constexpr size_t sizeRecvBuffer = 256;
static inline constexpr std::uintptr_t serverID = 0;
static inline constexpr std::uintptr_t clientIdOffset = 1;

//std::array<Client*, clientsNumber> everyClients{};
net::SocketPool everySockets{ clientsNumber };

std::array<ExContext*, clientsNumber> clientContexts{};
// all-in-one circular buffer
std::array<std::byte, clientsNumber* sizeRecvBuffer> clientsRecvBuffer{};

[[nodiscard]]
constexpr size_t GetIndexOnID(const std::uintptr_t& id) noexcept
{
	return id - clientIdOffset;
}

[[nodiscard]]
constexpr std::span<std::byte, sizeRecvBuffer>
GetBuffer(const std::uintptr_t& id)
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

	everySockets.Add(&serverListener, serverID);
	std::println("The listener is ready.");

	for (auto& ctx : clientContexts)
	{
		ctx = new ExContext{};
	}
	std::println("Contexts are ready.");

	for (size_t i = 0; i < clientsNumber; ++i)
	{
		const std::uintptr_t id = i + clientIdOffset;

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
	std::println("Worker {} is started", nth);

	while (true)
	{
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

					std::println("Client connected - {}", id);

					auto it = everySockets.Find(id);
					auto& client = *it;
					auto& socket = client.sk;
					op = IoOperation::Recv;

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
					if (0 == bytes)
					{
						std::println("Closing client {} as sending has been failed", id);

						auto it = everySockets.Find(id);
						auto& client = *it;
						auto& socket = client.sk;

						op = IoOperation::Close;
						socket->CloseAsync(ex_context);

						break; // switch (op)
					}

					auto buffer = GetBuffer(id);
					auto buffer_first_it = buffer.cbegin();
					auto buffer_last_it = buffer.cbegin() + bytes;

					std::string temp_msg{};
					temp_msg.reserve(static_cast<size_t>(bytes + 1));

					while (buffer_first_it != buffer_last_it)
					{
						temp_msg.push_back(static_cast<const char>(*buffer_first_it));
						++buffer_first_it;
					}

					struct MemoryFailsafe
					{
						~MemoryFailsafe()
						{
							if (not ok and ptr)
							{
								delete ptr;
							}
						}

						bool ok;
						ChatMsgContext* ptr;
					};

					auto msg_ctx = new ChatMsgContext{};
					MemoryFailsafe failsafe{ .ptr = msg_ctx };

					msg_ctx->chatMsg = std::move(temp_msg);
					msg_ctx->myID = id;
					msg_ctx->myOperation = IoOperation::BroadcastMessage;

					if (everySockets.Schedule(msg_ctx, id, bytes))
					{
						failsafe.ok = true;
					}
					else
					{
						std::println("Worker cannot schedule a broadcasting on the client {}", id);
					}

					auto it = everySockets.Find(id);
					auto& client = *it;
					auto& socket = client.sk;
					op = IoOperation::Recv;

					// Restart receive
					auto r = socket->Receive(*ex_context, buffer);

					if (r)
					{
						std::println("Client {}'s receive are reserved", id);
					}
					else
					{
						std::println("Client {}'s receive are not able to be reserved ({})", id, r.error());

						op = IoOperation::Close;
						socket->CloseAsync(ex_context);

						break; // switch (op)
					}
				}
				break;

				case IoOperation::Send:
				{
					const auto& bytes = io_event.ioBytes;

					if (not io_event.isSucceed)
					{
						std::println("Worker has been failed as sending on the client {}", id);

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

				// Custome event
				case IoOperation::BroadcastMessage:
				{
					auto msg_ctx = static_cast<ChatMsgContext*>(ex_context);
					if (nullptr == msg_ctx)
					{
						std::println("An msg error occured on the client {}", id);
						std::abort();
						break;
					}

					//constexpr char msg[]{ "asdasdda" };
					const auto& msg = msg_ctx->chatMsg;
					const auto msg_data = reinterpret_cast<const std::byte*>(msg.data());
					const size_t msg_size = msg.size();

					std::println("Client {}: {}", id, msg);

					for (auto& ck : everySockets)
					{
						const auto& target_id = ck.id;
						if (target_id == serverID)
						{
							continue;
						}

						auto& target_ctx = clientContexts[GetIndexOnID(ck.id)];
						auto& socket = *ck.sk;
						// IsAvailable은 안됨
						//if (not socket.IsAvailable())
						if (target_ctx->myOperation == IoOperation::None || target_ctx->myOperation == IoOperation::Accept)
						{
							continue;
						}

						auto sender = new ExContext{};
						sender->myID = target_id;
						sender->myOperation = IoOperation::Send;

						auto sr = socket.Send(*sender, msg_data, msg_size);

						if (not sr)
						{
							std::println("Client {} cannot send to message to {}", id, target_id);
						}
						else
						{
							std::println("Client {} to {}", id, target_id);
						}
					}

					delete msg_ctx;
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
