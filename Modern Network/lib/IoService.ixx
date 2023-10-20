export module Net.IoService;
import Net.IResourcePool;
import Net.IoContext;
import <thread>;

export namespace net
{
	class [[nodiscard]] IoService
	{
	public:
		IResourcePool<IoContext>* ioContexts;
		IResourcePool<std::jthread>* myThreads;
	};
}
