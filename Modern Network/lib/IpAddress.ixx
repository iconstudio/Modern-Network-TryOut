export module Net.IpAddress;
export import :IpAddressFamily;
import <type_traits>;
import <string_view>;
import <memory>;

export namespace net
{
	struct alignas(char) [[nodiscard]] SerializedIpAddress
	{
		constexpr SerializedIpAddress() noexcept = default;
		constexpr ~SerializedIpAddress() noexcept = default;

		char data[16];
	};

	class [[nodiscard]] IpAddress final
	{
	public:
		IpAddress(const IpAddressFamily& family, std::string_view address);
		IpAddress(const IpAddress& other);
		IpAddress& operator=(const IpAddress& other);
		~IpAddress() = default;

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
			return std::move(addressFamily);
		}
		[[nodiscard]]
		constexpr std::unique_ptr<char[]>&& GetAddress() && noexcept
		{
			return std::move(addressBuffer);
		}

		[[nodiscard]]
		constexpr bool operator==(const IpAddress&) const noexcept = default;

		constexpr IpAddress(IpAddress&&) noexcept = default;
		constexpr IpAddress& operator=(IpAddress&&) noexcept = default;

	private:
		[[nodiscard]]
		static constexpr size_t
			GetSizeOfFamilyBuffer(const IpAddressFamily& family)
			noexcept
		{
			if (IpAddressFamily::IPv4 == family)
			{
				return 16;
			}
			else if (IpAddressFamily::IPv6 == family)
			{
				return 48;
			}
			else
			{
				return 0;
			}
		}

		IpAddressFamily addressFamily;
		std::unique_ptr<char[]> addressBuffer;
	};
}
