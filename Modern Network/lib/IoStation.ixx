export module Net.Io.Station;
import Net.NativeHandle;
import Net.Handler;
import Net.Socket;
import Net.ErrorCode;
import Net.Io.Event;
import Net.Io.Schedule;
import <cstdint>;
import <limits>;
import <expected>;
import <memory>;
import <coroutine>;
import <stop_token>;

export namespace net::io
{
	class [[nodiscard]] Station final
		: public net::Handler<net::NativeHandle>, public std::enable_shared_from_this<Station>
	{
	private:
		struct Awaiter
		{
			static constexpr bool await_ready() noexcept
			{
				return true;
			}
			static constexpr void await_suspend(std::coroutine_handle<void>) noexcept
			{}
			[[nodiscard]] std::unique_ptr<net::io::Schedule> await_resume();

			net::io::Station& ioStation;
		};

	public:
		using Stationary = std::expected<net::io::Station, net::ErrorCodes>;

		Station() = default;
		~Station() = default;

		net::SocketResult Register(net::Socket& socket, std::uint64_t id) noexcept;
		bool TryRegister(net::Socket& socket, std::uint64_t id, net::ErrorCodes& error_code) noexcept;
		void Stop();
		bool Destroy() noexcept;
		bool Destroy(net::ErrorCodes& error_code) noexcept;

		[[nodiscard]] Awaiter Schedule() noexcept;

		[[nodiscard]] static Stationary Create() noexcept;
		[[nodiscard]] static Stationary Create(std::uint32_t concurrency_hint) noexcept;

		constexpr Station(Station&&) noexcept = default;
		constexpr Station& operator=(Station&&) noexcept = default;

	private:
		Station(net::NativeHandle&& handle) noexcept;

		[[nodiscard]] std::stop_token MakeCancelToken() const noexcept;

		Station(const Station&) = delete;
		Station& operator=(const Station&) = delete;

		std::stop_source mySwitch;
	};
}