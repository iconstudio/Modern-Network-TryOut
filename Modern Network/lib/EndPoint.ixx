module;
#include <cstdint>
#include <type_traits>
#include <string_view>

export module Net.EndPoint;
import Net.IpAddress;

export namespace net
{
	struct [[nodiscard]] EndPoint final
	{
	private:
		struct [[nodiscard]] EmptyEndPointType final
		{
			constexpr EmptyEndPointType() noexcept = default;
			constexpr ~EmptyEndPointType() noexcept = default;
		};

	public:
		static const EmptyEndPointType EmptyEndPoint;

		EndPoint(EmptyEndPointType) noexcept;
		EndPoint(const IpAddress& ip, const std::uint16_t& port) noexcept;
		EndPoint(IpAddress&& ip, const std::uint16_t& port) noexcept;
		~EndPoint() noexcept = default;

		[[nodiscard]]
		constexpr const IpAddressFamily& GetAddressFamily() const& noexcept
		{
			return myAddress.GetFamily();
		}
		[[nodiscard]]
		constexpr const IpAddress& GetIpAddress() const& noexcept
		{
			return myAddress;
		}
		[[nodiscard]]
		std::string_view GetAddress() const& noexcept
		{
			return myAddress.GetAddress();
		}
		[[nodiscard]]
		constexpr const std::uint16_t& GetPort() const& noexcept
		{
			return myPort;
		}
		[[nodiscard]]
		constexpr IpAddressFamily&& GetAddressFamily() && noexcept
		{
			return std::move(myAddress).GetFamily();
		}
		[[nodiscard]]
		constexpr IpAddress&& GetIpAddress() && noexcept
		{
			return std::move(myAddress);
		}
		[[nodiscard]]
		constexpr decltype(auto) GetAddress() && noexcept
		{
			return std::move(myAddress).GetAddress();
		}
		[[nodiscard]]
		constexpr std::uint16_t&& GetPort() && noexcept
		{
			return std::move(myPort);
		}

	private:
		IpAddress myAddress;
		std::uint16_t myPort;
	};
}
