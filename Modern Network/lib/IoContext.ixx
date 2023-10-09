export module Net.IoContext;
import Net.NativeHandle;
import Net.IHandler;

export namespace net
{
	class IoContext : public IHandler<NativeHandle>
	{
	public:
		constexpr IoContext() = default;
		virtual ~IoContext() = default;
	};
}
