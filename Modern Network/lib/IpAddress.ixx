export module Net.IpAddress;
import <string_view>;
import <memory>;
export import :IpAddressFamily;

export namespace net
{
	struct alignas(char) [[nodiscard]] SerializedIpAddress
	{
		constexpr SerializedIpAddress() noexcept = default;
		constexpr ~SerializedIpAddress() noexcept = default;

		char data[64];
	};

	class [[nodiscard]] IpAddress final
	{
	public:
		IpAddress(const IpAddressFamily& family, std::string_view address);
		IpAddress(const IpAddress& other);
		IpAddress& operator=(const IpAddress& other);

		~IpAddress()
		{}

		[[nodiscard]]
		SerializedIpAddress Serialize() const noexcept;
		bool TrySerialize(SerializedIpAddress& out) const noexcept;
		bool TrySerialize(void* const& out) const noexcept;

		[[nodiscard]]
		constexpr const IpAddressFamily& GetFamily() const& noexcept
		{
			return addressFamily;
		}
		[[nodiscard]]
		constexpr std::string_view GetAddress() const& noexcept
		{
			return addressBuffer.get();
		}
		[[nodiscard]]
		constexpr IpAddressFamily&& GetFamily() && noexcept
		{
			return static_cast<IpAddressFamily&&>(addressFamily);
		}
		[[nodiscard]]
		constexpr std::unique_ptr<char[]>&& GetAddress() && noexcept
		{
			return static_cast<std::unique_ptr<char[]>&&>(addressBuffer);
		}

		[[nodiscard]]
		constexpr bool operator==(const IpAddress&) const noexcept = default;

		constexpr IpAddress(IpAddress&&) noexcept = default;
		constexpr IpAddress& operator=(IpAddress&&) noexcept = default;

		IpAddressFamily addressFamily;
		std::unique_ptr<char[]> addressBuffer;
	};
}
