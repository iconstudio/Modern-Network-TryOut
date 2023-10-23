module;
#include <cstddef>
#include <cstdint>
#include <span>

export module Net.Socket;
export import :Result;
export import :Options;
export import Net.ErrorCode;
import Net.Handler;
import Net.Property;
import Net.IoContext;
import Net.Task;
import Net.IpAddress;
import Net.InternetProtocols;
import Net.EndPoint;

export namespace net
{
	using NativeSocket = std::uintptr_t;

	struct AttentSocket;
	struct SocketConnectum;

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

		using FactoryResult = expected<Socket, ErrorCodes>;

		// Static members

		static const EmptySocketType EmptySocket;

		// Constructors, Destructors, Assignees

		Socket(EmptySocketType) noexcept;
		Socket& operator=(EmptySocketType) noexcept;
		~Socket() noexcept;

		// Opt-in Interfaces

		SocketResult Bind(const IpAddress& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(IpAddress&& address, const std::uint16_t& port) const noexcept;
		SocketResult Bind(const EndPoint& endpoint) const noexcept;
		SocketResult Bind(EndPoint&& endpoint) const noexcept;

		bool ReusableAddress() const noexcept;
		void ReusableAddress(bool flag) noexcept;
		bool Close() noexcept;
		bool Close(SocketClosingErrorCodes& error_code) noexcept;

		// Opt-out Methods

		SocketListeningResult Open() const noexcept;
		SocketResult Connect(const IpAddress& address, const std::uint16_t& port) const noexcept;
		SocketResult Connect(IpAddress&& address, const std::uint16_t& port) const noexcept;
		SocketResult Connect(const EndPoint& endpoint) const noexcept;
		SocketResult Connect(EndPoint&& endpoint) const noexcept;
		[[nodiscard]]
		Task<SocketResult> ConnectAsync(const EndPoint& endpoint) const noexcept;
		[[nodiscard]]
		Task<SocketResult> ConnectAsync(EndPoint&& endpoint) const noexcept;
		[[nodiscard]]
		Task<SocketResult> ConnectAsync(const IpAddress& address, const std::uint16_t& port) const noexcept;
		[[nodiscard]]
		Task<SocketResult> ConnectAsync(IpAddress&& address, const std::uint16_t& port) const noexcept;

		// Synchronous Send

		SocketSendingResult Send(std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(_In_reads_bytes_(size)const std::byte* const& memory, size_t size) const noexcept;
		bool Send(std::span<const std::byte> memory, SendingErrorCodes& error_code) const noexcept;
		bool Send(_In_reads_bytes_(size)const std::byte* const& memory, size_t size, SendingErrorCodes& error_code) const noexcept;

		// Maybe asynchronous Send

		SocketSendingResult Send(_Maybenull_ IoContext* context, std::span<const std::byte> memory) const noexcept;
		SocketSendingResult Send(_Maybenull_ IoContext* context, _In_reads_bytes_(size)const std::byte* const& memory, size_t size) const noexcept;
		bool Send(_Maybenull_ IoContext* context, std::span<const std::byte> memory, SendingErrorCodes& error_code) const noexcept;
		bool Send(_Maybenull_ IoContext* context, _In_reads_bytes_(size)const std::byte* const& memory, size_t size, SendingErrorCodes& error_code) const noexcept;

		// Synchronous Receive

		SocketReceivingResult Receive(std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(_In_reads_bytes_(size)const std::byte* const& memory, size_t size) const noexcept;
		bool Receive(std::span<std::byte> memory, ReceivingErrorCodes& error_code) const noexcept;
		bool Receive(_In_reads_bytes_(size)const std::byte* const& memory, size_t size, ReceivingErrorCodes& error_code) const noexcept;

		// Maybe asynchronous Receive

		SocketReceivingResult Receive(_Maybenull_ IoContext* context, std::span<std::byte> memory) const noexcept;
		SocketReceivingResult Receive(_Maybenull_ IoContext* context, _In_reads_bytes_(size)const std::byte* const& memory, size_t size) const noexcept;
		bool Receive(_Maybenull_ IoContext* context, std::span<std::byte> memory, ReceivingErrorCodes& error_code) const noexcept;
		bool Receive(_Maybenull_ IoContext* context, _In_reads_bytes_(size)const std::byte* const& memory, size_t size, ReceivingErrorCodes& error_code) const noexcept;

		// Asynchronous Send & Receive

		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(_Maybenull_ IoContext* context, std::span<const std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketSendingResult> SendAsync(_Maybenull_ IoContext* context, const _In_reads_bytes_(size) std::byte* const& memory, size_t size) const noexcept;
		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(_Maybenull_ IoContext* context, std::span<std::byte> memory) const noexcept;
		[[nodiscard]]
		Task<SocketReceivingResult> ReceiveAsync(_Maybenull_ IoContext* context, _In_reads_bytes_(size) const std::byte* const& memory, size_t size) const noexcept;

		// Observers

		[[nodiscard]]
		bool IsAvailable() const noexcept;

		// Static methods

		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family) noexcept;
		[[nodiscard]]
		static Socket Create(const InternetProtocols& protocol, const IpAddressFamily& family, ErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static bool TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out) noexcept;
		[[nodiscard]]
		static bool TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family, AttentSocket& out, ErrorCodes& error_code) noexcept;
		[[nodiscard]]
		static FactoryResult TryCreate(const InternetProtocols& protocol, const IpAddressFamily& family) noexcept;
		static void SetAddressReusable(Socket& target, bool& flag) noexcept;

		// Default methods

		constexpr Socket(Socket&&) noexcept = default;
		constexpr Socket& operator=(Socket&&) noexcept = default;
		constexpr bool operator==(const Socket&) const noexcept = default;

		// Fields

		ReadonlyProperty<InternetProtocols> myProtocol;
		ReadonlyProperty<IpAddressFamily> myFamily;
		CustomProperty<bool, Socket> IsAddressReusable;

		friend class AttentSocket;

	private:
		Socket() noexcept;
		Socket(NativeSocket sock, InternetProtocols protocol, IpAddressFamily family) noexcept;

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
	};

	struct [[nodiscard]] AttentSocket
	{
		Socket Socket = Socket::EmptySocket;
	};

	struct [[nodiscard]] SocketConnectum
	{
		Socket Client;
		EndPoint Address;
	};
}
