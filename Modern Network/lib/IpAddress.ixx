export module Net.IpAddress;
export import :IpAddressFamily;
import <exception>;
import <type_traits>;
import <string_view>;
import <memory>;

export namespace net
{
	[[nodiscard]]
	constexpr size_t
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

	class [[nodiscard]] IpAddress final
	{
	public:
		constexpr IpAddress(const IpAddressFamily& family, std::string_view address)
			: addressFamily(family), addressBuffer()
		{
			const size_t sz = GetSizeOfFamilyBuffer(family);
			addressBuffer = std::make_unique<char[]>(sz);

			std::copy_n(address.begin(), sz, addressBuffer.get());
		}

		constexpr IpAddress(const IpAddress& other)
			: addressFamily(other.addressFamily), addressBuffer()
		{
			const size_t sz = GetSizeOfFamilyBuffer(other.addressFamily);
			addressBuffer = std::make_unique<char[]>(sz);

			std::copy_n(other.addressBuffer.get(), sz, addressBuffer.get());
		}

		constexpr IpAddress& operator=(const IpAddress& other)
		{
			if (addressBuffer)
			{
				if (other.addressFamily != addressFamily)
				{
					throw std::exception("Cannot assign to the address with a unmatched address family!");
				}

				std::copy_n(other.addressBuffer.get(), GetSizeOfFamilyBuffer(addressFamily), addressBuffer.get());
				return *this;
			}
			else
			{
				return *this = IpAddress(other);
			}
		}

		constexpr ~IpAddress() = default;

		bool Serialize() noexcept;
		[[nodiscard]]
		constexpr IpAddressFamily GetFamily() const& noexcept
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
		IpAddressFamily addressFamily;
		std::unique_ptr<char[]> addressBuffer;
	};
}
