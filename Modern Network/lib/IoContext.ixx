export module Net.Io.Context;
import Net.NativeHandle;
import <cstdint>;

export namespace net::io
{
	class [[nodiscard]] Context
	{
	public:
		constexpr Context() noexcept = default;
		constexpr ~Context() noexcept
		{}

		void Clear() noexcept
		{
			ioLower = 0;
			ioUpper = 0;

			std::uintptr_t* repr = reinterpret_cast<std::uintptr_t*>(std::addressof(offset));
			*repr = 0ULL;

			eventObject = nullptr;
		}

	protected:
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
	};
}
