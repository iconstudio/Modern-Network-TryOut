export module Net.Io.Schedule;
import Net.Io.Event;
import <stop_token>;
import <coroutine>;

export namespace net::io
{
	class Station;
	struct CancellationToken {};

	class [[nodiscard]] Schedule final
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
			net::io::Event& await_resume() noexcept;

			net::io::Schedule& ioSchedule;
		};

	public:
		Schedule(net::io::Station& station, std::stop_token&& token) noexcept
			: ioStation(station), ioResult()
			, myCancelToken(std::move(token))
		{}

		Awaiter Start();

		[[nodiscard]]
		net::io::Event& GetEvent() noexcept
		{
			return ioResult;
		}

		[[nodiscard]]
		const net::io::Event& GetEvent() const noexcept
		{
			return ioResult;
		}

		[[nodiscard]]
		bool IsCancelled() const noexcept
		{
			return myCancelToken.stop_requested();
		}

		constexpr bool operator==(const Schedule&) const noexcept = default;

	private:
		Schedule(const Schedule&) = delete;
		Schedule(Schedule&&) = delete;
		Schedule& operator=(const Schedule&) = delete;
		Schedule& operator=(Schedule&&) = delete;

		net::io::Station& ioStation;
		net::io::Event ioResult;
		std::stop_token myCancelToken;
	};
}
