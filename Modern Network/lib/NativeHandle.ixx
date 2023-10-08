export module Net.NativeHandle;
import <compare>;

export namespace net
{
	class [[nodiscard]] NativeHandle
	{
	public:
		constexpr NativeHandle() noexcept = default;
		constexpr ~NativeHandle() noexcept = default;

		[[nodiscard]]
		constexpr const void* const GetPointer() const noexcept
		{
			return nativePointer;
		}

		[[nodiscard]]
		static constexpr NativeHandle Create(void* const& handle) noexcept
		{
			return NativeHandle{ handle };
		}

		constexpr bool operator==(const NativeHandle&) const noexcept = default;
		constexpr std::strong_ordering operator<=>(const NativeHandle&) const noexcept = default;

		constexpr NativeHandle(NativeHandle&&) noexcept = default;
		constexpr NativeHandle& operator=(NativeHandle&&) noexcept = default;

	private:
		constexpr NativeHandle(void* const& handle) noexcept
			: nativePointer(handle)
		{}

		NativeHandle(const NativeHandle&) = delete;
		NativeHandle& operator=(const NativeHandle&) = delete;

		void* nativePointer;
	};
}
