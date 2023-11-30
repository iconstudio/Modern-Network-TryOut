export module Net.Io.Station;
import Net.NativeHandle;
import Net.Handler;
import Net.Socket;
import Net.ErrorCode;
import Net.Io.Event;
import <cstdint>;
import <limits>;
import <expected>;
import <coroutine>;

export namespace net::io
{
	class [[nodiscard]] Station final
		: public Handler<NativeHandle>
	{
	private:
		struct Awaiter
		{
			static constexpr bool await_ready() noexcept
			{
				return false;
			}

			std::coroutine_handle<void> await_suspend(std::coroutine_handle<void> previous_frame) noexcept;
			net::io::Event await_resume() noexcept;

			net::io::Station& ioStation;
			net::io::Event myResult;
		};

	public:
		using Stationary = std::expected<net::io::Station, net::ErrorCodes>;

		constexpr Station() noexcept = default;
		~Station() noexcept;

		SocketResult Register(Socket& socket, std::uint64_t id) noexcept;
		bool TryRegister(Socket& socket, std::uint64_t id, ErrorCodes& error_code) noexcept;
		bool Destroy() noexcept;
		bool Destroy(ErrorCodes& error_code) noexcept;

		Awaiter operator co_await() noexcept;

		[[nodiscard]] static Stationary Create() noexcept;
		[[nodiscard]] static Stationary Create(std::uint32_t concurrency_hint) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		constexpr Station(NativeHandle&& handle) noexcept;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;
	};
}