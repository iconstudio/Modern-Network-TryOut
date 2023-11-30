export module Net.Io.Station;
import Net.NativeHandle;
import Net.Handler;
import Net.Socket;
import Net.ErrorCode;
import <cstdint>;
import <limits>;
import <expected>;

export namespace net::io
{
	class [[nodiscard]] Station final : public Handler<NativeHandle>
	{
	public:
		using Stationary = std::expected<Station, ErrorCodes>;

		constexpr Station() noexcept = default;
		~Station() noexcept;

		SocketResult Register(Socket& socket, std::uint64_t id) const noexcept;
		bool TryRegister(Socket& socket, std::uint64_t id, ErrorCodes& error_code) const noexcept;

		[[nodiscard]] static Stationary Create() noexcept;
		[[nodiscard]] static Stationary Create(std::uint32_t concurrency_hint) noexcept;
		[[nodiscard]] static Stationary Create(std::uint64_t id) noexcept;
		[[nodiscard]] static Stationary Create(std::uint64_t id, std::uint32_t concurrency_hint) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		static constexpr std::uint64_t defaultID = std::numeric_limits<std::uint64_t>::max();

		constexpr Station(NativeHandle&& handle) noexcept;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;
	};
}