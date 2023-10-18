export module Net.NativeHandle;
import Net.Property;

export namespace net
{
	class [[nodiscard]] NativeHandle final
	{
	public:
		constexpr NativeHandle() noexcept = default;
		constexpr ~NativeHandle() noexcept = default;

		[[nodiscard]]
		constexpr const void* const& GetPointer() const& noexcept
		{
			return nativePointer;
		}

		[[nodiscard]]
		constexpr void* && GetPointer() && noexcept
		{
			return static_cast<void* &&>(nativePointer);
		}

		[[nodiscard]]
		static constexpr NativeHandle Create(void* const& handle) noexcept
		{
			return NativeHandle{ handle };
		}

		constexpr bool operator==(const NativeHandle&) const noexcept = default;

		constexpr NativeHandle(NativeHandle&&) noexcept = default;
		constexpr NativeHandle& operator=(NativeHandle&&) noexcept = default;

	private:
		constexpr NativeHandle(void* const& handle) noexcept
			: nativePointer(handle)
		{}

		NativeHandle(const NativeHandle&) = delete;
		NativeHandle& operator=(const NativeHandle&) = delete;

		Property<void*> nativePointer;
	};
}
