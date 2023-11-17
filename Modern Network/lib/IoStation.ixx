export module Net.Io.Station;
import Net.NativeHandle;
import Net.Handler;
import Net.Socket;
import Net.ErrorCode;
import <cstdint>;
import <expected>;

export namespace net::io
{
	class [[nodiscard]] Station final : public Handler<NativeHandle>
	{
	public:
		using Stationary = std::expected<Station, ErrorCodes>;

		~Station() noexcept;

		SocketResult Register(Socket& socket) noexcept;
		bool TryRegister(Socket& socket, ErrorCodes& error_code) noexcept;

		static Stationary Create() noexcept;
		static Stationary Create(std::uint32_t concurrency_hint) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		constexpr Station() noexcept = default;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;
	};
}