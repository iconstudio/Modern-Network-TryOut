#pragma comment(lib, "Modern Network.lib")
#include <cstdlib>
#include <array>
#include <vector>
#include <span>
#include <print>
#include <string>
#include <thread>
#include <algorithm>
#include <atomic>

import Net;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.SocketPool;

import Test.IoOperation;
import Test.Context;
import Test.Client;

void Worker(size_t nth);

class ChatMsgContext : public test::ExContext
{
public:
	using ExContext::ExContext;

	std::atomic_int refCount;
	std::string chatMsg;
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

struct PointerPacket : public BasicPacket
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
static inline volatile std::atomic_size_t clientsNumber = 0;
static inline constexpr size_t maxClientsNumber = 40;
static inline constexpr size_t sizeRecvBuffer = 256;
static inline constexpr std::uintptr_t serverID = 0;
static inline constexpr std::uintptr_t clientIdOffset = 1;

net::SocketPool everySockets{ maxClientsNumber };

std::array<test::Client*, maxClientsNumber> everyClients{};
// all-in-one circular buffer
std::array<std::byte, maxClientsNumber* sizeRecvBuffer> clientsRecvBuffer{};

[[nodiscard]]
constexpr size_t GetIndexByID(const std::uintptr_t& id) noexcept
{
	return id - clientIdOffset;
}

[[nodiscard]]
constexpr std::uintptr_t GetIDByIndex(const size_t& index) noexcept
{
	return index + clientIdOffset;
}

[[nodiscard]]
constexpr test::Client& FindClient(const std::uintptr_t& id)
{
	return *everyClients[GetIndexByID(id)];
}

[[nodiscard]]
constexpr std::span<std::byte, sizeRecvBuffer>
GetBuffer(const std::uintptr_t& id)
noexcept
{
	return std::span<std::byte, sizeRecvBuffer>(clientsRecvBuffer.data() + GetIndexByID(id) * sizeRecvBuffer, sizeRecvBuffer);
}

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

	size_t client_index = 0;
	for (auto* & client : everyClients)
	{
		const auto id = GetIDByIndex(client_index);

		client = new test::Client{};
		client->myID = id;
		client->mySocket = everySockets.Allocate(id, net::SocketType::Asynchronous, net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);
		client->mySocket->IsAddressReusable = true;

		auto& ctx = client->myContext;
		ctx.myID = id;
		ctx.myOperation = test::IoOperation::None;

		++client_index;
	}
	std::println("Client prefabs are ready.");

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

	for (auto*& client : everyClients)
	{
		auto& ctx = client->myContext;
		ctx.myOperation = test::IoOperation::Accept;

		auto acceptance = serverListener.ReserveAccept(ctx, *client->mySocket);
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

		auto ex_context = static_cast<test::ExContext*>(io_context);
		if (ex_context)
		{
			ex_context->Clear();

			auto& op = ex_context->myOperation;
			const auto& id = ex_context->myID;

			switch (op)
			{
				case test::IoOperation::Accept:
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
					op = test::IoOperation::Recv;

					auto r = socket->Receive(*ex_context, GetBuffer(id));

					if (r)
					{
						std::println("Client {}'s receive are reserved", id);
						++clientsNumber;
					}
					else
					{
						std::println("Client {}'s receive are not able to be reserved ({})", id, r.error());
					}
				}
				break;

				case test::IoOperation::Recv:
				{
					auto it = everySockets.Find(id);
					auto& client = *it;
					auto& socket = client.sk;

					if (not io_event.isSucceed)
					{
						std::println("Worker has been failed as receiving on client {}", id);

						op = test::IoOperation::Close;
						socket->CloseAsync(ex_context);

						break; // switch (op)
					}

					auto& bytes = io_event.ioBytes;
					if (0 == bytes)
					{
						std::println("Closing client {} as sending has been failed", id);

						op = test::IoOperation::Close;
						socket->CloseAsync(ex_context);

						break; // switch (op)
					}

					auto buffer = GetBuffer(id);
					auto buffer_first_it = buffer.cbegin();
					auto buffer_last_it = buffer.cbegin() + bytes;

					std::string temp_msg{};
					temp_msg.reserve(12 + static_cast<size_t>(bytes) * 2);

					temp_msg += std::format("Client {}: ", id);
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
					msg_ctx->myOperation = test::IoOperation::BroadcastMessage;
					msg_ctx->refCount = static_cast<int>(clientsNumber.load(std::memory_order_relaxed));

					if (everySockets.Schedule(msg_ctx, id, bytes))
					{
						failsafe.ok = true;
					}
					else
					{
						std::println("Worker cannot schedule a broadcasting on the client {}", id);
					}

					op = test::IoOperation::Recv;

					// Restart receive
					auto r = socket->Receive(*ex_context, buffer);

					if (r)
					{
						std::println("Client {}'s receive are reserved", id);
					}
					else
					{
						std::println("Client {}'s receive are not able to be reserved ({})", id, r.error());

						op = test::IoOperation::Close;
						socket->CloseAsync(ex_context);

						break; // switch (op)
					}
				}
				break;

				case test::IoOperation::Send:
				{
					const auto& bytes = io_event.ioBytes;

					if (not io_event.isSucceed)
					{
						std::println("Worker has been failed as sending on the client {}", id);

						if (ex_context != nullptr)
						{
							delete ex_context;
						}

						if (0 == bytes)
						{
							std::println("Closing client {} as sending has been failed", id);

							auto& client = FindClient(id);
							auto& socket = *client.mySocket;

							auto& ctx = client.myContext;
							ctx.myOperation = test::IoOperation::Close;

							socket.CloseAsync(ctx);
						}

						break; // switch (op)
					}

					// Specialization for chat msgs
					auto msg_ctx = static_cast<ChatMsgContext*>(ex_context);
					if (nullptr != msg_ctx)
					{
						std::println("Message sent to the client {}", id);

						// Preserve the message buffer until they sent
						if (msg_ctx->refCount.fetch_sub(1) <= 1)
						{
							delete msg_ctx;
						}
					}
					else
					{
						delete ex_context;
					}
				}
				break;

				// Custome event
				case test::IoOperation::BroadcastMessage:
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

					for (auto& ck : everySockets)
					{
						const auto& target_id = ck.id;
						if (target_id == serverID)
						{
							continue;
						}

						auto& target = FindClient(ck.id);
						auto& target_ctx = target.myContext;
						auto& socket = *ck.sk;

						// IsAvailable은 안됨
						// 
						//if (not socket.IsAvailable())
						constexpr auto range = std::array{ test::IoOperation::None, test::IoOperation::Accept };
						if (std::ranges::any_of(range, [&](const test::IoOperation& ops) noexcept {
								return target_ctx.myOperation == ops;
							}))
						{
							continue;
						}

						// Do not create another context
						//auto sender = new test::ExContext{};
						auto sender = msg_ctx;
						//sender->myID = target_id;
						sender->myOperation = test::IoOperation::Send;

						auto sr = socket.Send(*sender, msg_data, msg_size);

						if (not sr)
						{
							std::println("Client {} cannot send to message to {}", id, target_id);
						}
						else
						{
							std::println("Client {} to {}: {}", id, target_id, msg);
						}
					}

					//delete msg_ctx;
				}
				break;

				case test::IoOperation::Close:
				{
					ex_context->Clear();
					--clientsNumber;

					std::println("Client {} is closed", id);

					// accept again
					op = test::IoOperation::Accept;
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
