export module Net.IoService;
import Net.Io.Entity;
import Net.Io.Context;
import <vector>;
import <thread>;

export namespace net::io
{
	class [[nodiscard]] Service
	{
	public:
		Service() noexcept = default;
		~Service() noexcept = default;

		std::vector<Entity> ioContexts;
		std::vector<std::jthread> myThreads;
	};
}
