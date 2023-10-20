export module Net.IoContext;
import Net.NativeHandle;
import <cstdint>;

export namespace net
{
	class IoContext
	{
	public:
		constexpr IoContext() = default;
		virtual ~IoContext() = default;

		std::uint64_t ioLower;
		std::uint64_t ioUpper;
		union
		{
			struct
			{
				std::uint32_t offsetLower;
				std::uint32_t offsetUpper;
			};
			NativeHandle offset;
		};
		NativeHandle eventObject;
	};
}
