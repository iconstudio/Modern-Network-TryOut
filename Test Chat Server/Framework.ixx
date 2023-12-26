export module Test.Framework;
import <cstdlib>;
import <string>;
import <array>;
import <vector>;
import <span>;
import <thread>;
import <atomic>;

export import Net;
import Net.IpAddress;
export import Net.EndPoint;
export import Net.Socket;
export import Net.SocketPool;
export import Net.IpAddress.IPv4;

export import Test.IoOperation;
export import Test.Context;
export import Test.Client;

export namespace test
{
	struct ServerPreset
	{
		std::uintptr_t serverID;
		net::EndPoint serverAddress{ net::EndPoint::EmptyEndPoint };
	};

	class Framework final
	{
	public:
		static constexpr std::size_t maxClientsNumber = 40;
		static constexpr std::size_t sizeRecvBuffer = 256;
		static constexpr std::size_t oneBufferSize = maxClientsNumber * sizeRecvBuffer;
		static constexpr std::uintptr_t clientIdOffset = 1;

		Framework() noexcept;
		~Framework() noexcept;

		/// <summary>
		/// Initialize the framework
		/// </summary>
		/// <param name="setup">Settings of server</param>
		void Awake(ServerPreset&& setup);
		/// <summary>
		/// Start update
		/// </summary>
		/// <param name="concurrent_hint">Number of workers</param>
		void Start(size_t concurrent_hint);
		/// <summary>
		/// Remove framework's resource
		/// </summary>
		void Destroy();
		/// <summary>
		/// Clean every system resources up
		/// </summary>
		void Cleanup();

		[[nodiscard]]
		constexpr test::Client& FindClient(const std::uintptr_t& id) const
		{
			return *everyClients[GetIndexByID(id)];
		}

		[[nodiscard]]
		static constexpr size_t GetIndexByID(const std::uintptr_t& id) noexcept
		{
			return id - clientIdOffset;
		}

		[[nodiscard]]
		static constexpr std::uintptr_t GetIDByIndex(const size_t& index) noexcept
		{
			return index + clientIdOffset;
		}

	private:
		[[nodiscard]]
		std::span<std::byte, sizeRecvBuffer> GetBuffer(const std::uintptr_t& id) noexcept
		{
			std::byte* buf = clientsRecvBuffer.data();
			auto* data = buf + GetIndexByID(id) * sizeRecvBuffer;
			return std::span<std::byte, sizeRecvBuffer>(data, sizeRecvBuffer);
		}

		[[nodiscard]]
		std::span<const std::byte, sizeRecvBuffer> GetBuffer(const std::uintptr_t& id) const noexcept
		{
			const std::byte* buf = clientsRecvBuffer.data();
			auto* data = buf + GetIndexByID(id) * sizeRecvBuffer;
			return std::span<const std::byte, sizeRecvBuffer>(data, sizeRecvBuffer);
		}

		friend void Worker(Framework&, size_t);

		Framework(const Framework&) = delete;
		Framework(Framework&&) = delete;
		void operator=(const Framework&) = delete;
		void operator=(Framework&&) = delete;

		net::Socket myListener;
		std::array<test::Client*, maxClientsNumber> everyClients;
		net::SocketPool everySockets;
		// all-in-one circular buffer
		std::array<std::byte, oneBufferSize> clientsRecvBuffer;
		volatile std::atomic_size_t clientsNumber;
		std::vector<std::jthread> myWorkers{};
	};

	void Worker(Framework& framework, size_t nth);

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
}
