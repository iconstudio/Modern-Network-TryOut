export module Net.Socket;
export import Net.Socket.ErrorCode;
export import :Result;
import Net.Handler;
import Net.Property;
import Net.IoContext;
import Net.Task;
import Net.IpAddress;
import Net.InternetProtocols;
import Net.EndPoint;
import <cstddef>;
import <cstdint>;
import <span>;
import <expected>;

export namespace net
{
	using NativeSocket = std::uintptr_t;

	class AttentSocket;

	class [[nodiscard]] Socket final : public Handler<NativeSocket>
	{
	private:
		struct [[nodiscard]] EmptySocketType final
		{
			constexpr EmptySocketType() noexcept = default;
			constexpr ~EmptySocketType() noexcept = default;
		};

	public:
		// Type members

		using FactoryResult = std::expected<Socket, SocketErrorCodes>;

		// Static members

		static const EmptySocketType EmptySocket;

		// Constructors, Destructors, Assignees

		Socket(EmptySocketType) noexcept;
		Socket& operator=(EmptySocketType) noexcept;
		~Socket() noexcept;

		// Opt-in interfaces

		SocketResult Bind(const IpAddress& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(IpAddress&& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(const EndPoint& endpoint) const noexcept;
		SocketResult Bind(EndPoint&& endpoint) const noexcept;

		// Methods

		SocketListeningResult Open() const noexcept;
		bool Close() noexcept;
		bool Close(SocketClosingErrorCodes& error_code) noexcept;

		// Synchronous Send & Receive (1)

		SocketSendingResult Send(std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(const std::byte* const& memory, size_t size) const noexcept;
		SocketReceivingResult Receive(std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(const std::byte* const& memory, size_t size) const noexcept;

		// Synchronous Send & Receive (2)

		bool Send(std::span<const std::byte> memory, SocketSendingResult& error) const noexcept;
		bool Send(const std::byte* const& memory, size_t size, SocketSendingResult& error) const noexcept;
		bool Receive(std::span<std::byte> memory, SocketReceivingResult& error) const noexcept;
		bool Receive(const std::byte* const& memory, size_t size, SocketReceivingResult& error) const noexcept;

		// Maybe asynchronous Send & Receive (1)

		SocketSendingResult Send(IoContext* context, std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;
		SocketReceivingResult Receive(IoContext* context, std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;

		// Maybe asynchronous Send & Receive (2)

		bool Send(IoContext* context, std::span<const std::byte> memory, SocketSendingResult& error) const noexcept;
		bool Send(IoContext* context, const std::byte* const& memory, size_t size, SocketSendingResult& error) const noexcept;
		bool Receive(IoContext* context, std::span<std::byte> memory, SocketReceivingResult& error) const noexcept;
		bool Receive(IoContext* context, const std::byte* const& memory, size_t size, SocketReceivingResult& error) const noexcept;

		// Asynchronous Send & Receive (1)

		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(IoContext* context, std::span<const std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;
		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(IoContext* context, std::span<std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;

		// Observers

		[[nodiscard]]
		bool IsAvailable() const noexcept;

		// Static methods

		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family) noexcept;
		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family, SocketErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static bool TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out) noexcept;
		[[nodiscard]]
		static bool TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out, SocketErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static FactoryResult TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family) noexcept;

		// Default methods

		constexpr Socket(Socket&&) noexcept = default;
		constexpr Socket& operator=(Socket&&) noexcept = default;
		constexpr bool operator==(const Socket&) const noexcept = default;

		// Fields

		ReadonlyProperty<InternetProtocols> myProtocol;
		ReadonlyProperty<IpAddressFamily> myFamily;

		friend class AttentSocket;

	private:
		Socket() noexcept;
		Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept;

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
	};

	class [[nodiscard]] AttentSocket
	{
	public:
		Socket Socket = Socket::EmptySocket;
	};
}
