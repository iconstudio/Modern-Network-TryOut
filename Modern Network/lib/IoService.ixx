export module Net.IoService;
import Net.IResourcePool;
import Net.Io.Context;
import <thread>;

export namespace net::io
{
	class [[nodiscard]] IoService
	{
	public:
		IResourcePool<Context>* ioContexts;
		IResourcePool<std::jthread>* myThreads;
	};
}
