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
		static const EmptySocketType EmptySocket;

		Socket(EmptySocketType) noexcept;
		Socket& operator=(EmptySocketType) noexcept;
		~Socket() noexcept;

		SocketResult Bind(const IpAddress& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(IpAddress&& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(const EndPoint& endpoint) const noexcept;
		SocketResult Bind(EndPoint&& endpoint) const noexcept;
		SocketListeningResult Open() const noexcept;
		bool Close() noexcept;

		SocketSendingResult Send(std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(const std::byte* const& memory, size_t size) const noexcept;
		SocketReceivingResult Receive(std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(const std::byte* const& memory, size_t size) const noexcept;

		bool Send(std::span<const std::byte> memory, SocketSendingResult& error) const noexcept;
		bool Send(const std::byte* const& memory, size_t size, SocketSendingResult& error) const noexcept;
		bool Receive(std::span<std::byte> memory, SocketReceivingResult& error) const noexcept;
		bool Receive(const std::byte* const& memory, size_t size, SocketReceivingResult& error) const noexcept;

		SocketSendingResult Send(IoContext* context, std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;
		SocketReceivingResult Receive(IoContext* context, std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;

		bool Send(IoContext* context, std::span<const std::byte> memory, SocketSendingResult& error) const noexcept;
		bool Send(IoContext* context, const std::byte* const& memory, size_t size, SocketSendingResult& error) const noexcept;
		bool Receive(IoContext* context, std::span<std::byte> memory, SocketReceivingResult& error) const noexcept;
		bool Receive(IoContext* context, const std::byte* const& memory, size_t size, SocketReceivingResult& error) const noexcept;

		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(IoContext* context, std::span<const std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;

		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(IoContext* context, std::span<std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(IoContext* context, const std::byte* const& memory, size_t size) const noexcept;

		[[nodiscard]]
		bool IsAvailable() const noexcept;

		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family) noexcept;
		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family, SocketErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static bool TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out, SocketErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static SocketErrorCodes TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out) noexcept;

		constexpr Socket(Socket&&) noexcept = default;
		constexpr Socket& operator=(Socket&&) noexcept = default;

		ReadonlyProperty<InternetProtocols> myProtocol;
		ReadonlyProperty<IpAddressFamily> myFamily;

		friend class AttentSocket;

	private:
		constexpr Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept;

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
	};

	class [[nodiscard]] AttentSocket
	{
	public:
		Socket Socket = Socket::EmptySocket;
	};
}
