export module Net.IoContext;
import Net.NativeHandle;
import Net.Handler;

export namespace net
{
	class IoContext : public Handler<NativeHandle>
	{
	public:
		constexpr IoContext() = default;
		virtual ~IoContext() = default;
	};
}
