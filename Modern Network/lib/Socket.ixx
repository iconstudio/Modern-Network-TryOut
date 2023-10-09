export module Net.Socket;
export import Net.Socket.ErrorCode;
export import :Result;
import Net.NativeHandle;
import Net.Handler;
import Net.IoContext;
import Net.Task;
import Net.EndPoint;
import <cstddef>;
import <span>;

export namespace net
{
	class [[nodiscard]] Socket final : public Handler<NativeHandle>
	{
	private:
		struct [[nodiscard]] EmptySocketType final
		{};
	public:
		static const EmptySocketType EmptySocket;

		constexpr Socket(EmptySocketType) noexcept;
		constexpr Socket& operator=(EmptySocketType) noexcept;
		~Socket() noexcept;

		SocketResult Bind(const EndPoint& endpoint) const noexcept;
		SocketResult Bind(EndPoint&& endpoint) const noexcept;

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
		static Socket Create() noexcept;
		[[nodiscard]]
		static Socket Create(int& error_code) noexcept;

		constexpr Socket(Socket&&) noexcept = default;
		constexpr Socket& operator=(Socket&&) noexcept = default;

	private:
		Socket() noexcept = default;

		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;
	};
}
