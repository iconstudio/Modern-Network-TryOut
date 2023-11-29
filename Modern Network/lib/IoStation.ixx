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

		constexpr Station() noexcept = default;
		~Station() noexcept;

		SocketResult Register(Socket& socket, std::uint64_t id) noexcept;
		bool TryRegister(Socket& socket, std::uint64_t id, ErrorCodes& error_code) noexcept;

		static Stationary Create() noexcept;
		static Stationary Create(std::uint32_t concurrency_hint) noexcept;
		static Stationary Create(std::uint64_t id) noexcept;
		static Stationary Create(std::uint64_t id, std::uint32_t concurrency_hint) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		constexpr Station() noexcept = default;
		constexpr Station(NativeHandle&& handle) noexcept;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;
	};
}