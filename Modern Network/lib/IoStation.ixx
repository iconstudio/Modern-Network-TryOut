export module Net.Io.Station;
import Net.NativeHandle;
import Net.Handler;
import Net.Socket;
import Net.ErrorCode;
import <cstdint>;

export namespace net::io
{
	class [[nodiscard]] Station final : public Handler<NativeHandle>
	{
	public:
		~Station() noexcept;

		SocketResult Register(Socket& socket) noexcept;
		bool TryRegister(Socket& socket, ErrorCodes& error_code) noexcept;

		static Station Create() noexcept;
		static Station Create(ErrorCodes& error_code) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		constexpr Station() noexcept = default;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;
	};
}