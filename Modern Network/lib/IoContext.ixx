export module Net.IoContext;
import Net.NativeHandle;
import <cstdint>;

export namespace net
{
	class IoContext
	{
	public:
		constexpr IoContext() noexcept = default;
		constexpr ~IoContext() noexcept
		{}

		std::uint64_t ioLower;
		std::uint64_t ioUpper;
		union
		{
			struct
			{
				std::uint32_t offsetLower;
				std::uint32_t offsetUpper;
			};
			NativeHandle offset = {};
		};
		NativeHandle eventObject;

		void Clear() noexcept
		{
			ioLower = 0;
			ioUpper = 0;

			int* repr = reinterpret_cast<int*>(std::addressof(offset));
			*repr = 0;

			eventObject = nullptr;
		}
	};
}
