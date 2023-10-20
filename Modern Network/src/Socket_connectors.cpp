module;
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <ws2ipdef.h>

module Net.Socket;

::SOCKADDR_STORAGE SerializeEndpoint(const net::EndPoint& endpoint) noexcept;
::SOCKADDR_STORAGE SerializeEndpoint(net::EndPoint&& endpoint) noexcept;

net::SocketResult
net::Socket::Bind(const net::IpAddress& address, const std::uint16_t& port)
const noexcept
{
	return this->Bind(EndPoint(address, port));
}

net::SocketResult
net::Socket::Bind(net::IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	return this->Bind(EndPoint(std::move(address), port));
}

net::SocketResult
net::Socket::Bind(const net::EndPoint& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(endpoint);
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::bind(myHandle, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)))
	{
		return unexpected(AcquireNetworkError());
	}

	return static_cast<unsigned int>(sizeof(sockaddr));
}

net::SocketResult
net::Socket::Bind(net::EndPoint&& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(std::move(endpoint));
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::bind(myHandle, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)))
	{
		return unexpected(AcquireNetworkError());
	}

	return static_cast<unsigned int>(sizeof(sockaddr));
}

net::SocketResult
net::Socket::Connect(const net::IpAddress& address, const std::uint16_t& port)
const noexcept
{
	return this->Connect(EndPoint(address, port));
}

net::SocketResult
net::Socket::Connect(net::IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	return this->Connect(EndPoint(std::move(address), port));
}

net::SocketResult
net::Socket::Connect(const net::EndPoint& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(endpoint);
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::WSAConnect(myHandle
		, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)
		, nullptr, nullptr
		, nullptr, nullptr))
	{
		auto error_code = AcquireNetworkError();
		if (error_code == ErrorCodes::NonBlockedOperation)
		{
			return 0;
		}
		else
		{
			return unexpected(error_code);
		}
	}

	return 1;
}

net::SocketResult
net::Socket::Connect(net::EndPoint&& endpoint)
const noexcept
{
	SOCKADDR_STORAGE sockaddr = SerializeEndpoint(std::move(endpoint));
	SOCKADDR_STORAGE* ptr = std::addressof(sockaddr);

	if (0 != ::WSAConnect(myHandle
		, reinterpret_cast<const SOCKADDR*>(ptr), sizeof(sockaddr)
		, nullptr, nullptr
		, nullptr, nullptr))
	{
		auto error_code = AcquireNetworkError();
		if (error_code == ErrorCodes::NonBlockedOperation)
		{
			return 0;
		}
		else
		{
			return unexpected(error_code);
		}
	}

	return 1;
}

net::Task<net::SocketResult>
net::Socket::ConnectAsync(const net::EndPoint& endpoint)
const noexcept
{
	co_return this->Connect(endpoint);
}

net::Task<net::SocketResult>
net::Socket::ConnectAsync(net::EndPoint&& endpoint)
const noexcept
{
	co_return this->Connect(std::move(endpoint));
}

net::Task<net::SocketResult>
net::Socket::ConnectAsync(const net::IpAddress& address, const std::uint16_t& port)
const noexcept
{
	co_return this->Connect(EndPoint(address, port));
}

net::Task<net::SocketResult>
net::Socket::ConnectAsync(net::IpAddress&& address, const std::uint16_t& port)
const noexcept
{
	co_return this->Connect(EndPoint(std::move(address), port));
}

[[nodiscard]]
::SOCKADDR_STORAGE
SerializeEndpoint(const net::EndPoint& endpoint)
noexcept
{
	const auto& ip = endpoint.GetIpAddress();
	const auto& port = endpoint.GetPort();

	SOCKADDR_STORAGE result{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(result);

	switch (endpoint.GetAddressFamily())
	{
		case net::IpAddressFamily::IPv4:
		{
			IN_ADDR sk_addr{};
			if (not ip.TrySerialize(std::addressof(sk_addr.s_addr)))
			{
				break;
			}

			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (std::uint16_t)net::IpAddressFamily::IPv4,
				.sin_port = port,
				.sin_addr = std::move(sk_addr),
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case net::IpAddressFamily::IPv6:
		{
			IN6_ADDR sk_addr{};
			if (not ip.TrySerialize(std::addressof(sk_addr.s6_addr)))
			{
				break;
			}

			SOCKADDR_IN6 ipv6_addr
			{
				.sin6_family = (std::uint16_t)net::IpAddressFamily::IPv6,
				.sin6_port = port,
				.sin6_flowinfo = 0,
				.sin6_addr = std::move(sk_addr),
				.sin6_scope_id = 0,
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv6_addr));
		}
		break;

		case net::IpAddressFamily::Unknown:
		{}
		break;
	}

	return result;
}

[[nodiscard]]
::SOCKADDR_STORAGE
SerializeEndpoint(net::EndPoint&& endpoint)
noexcept
{
	SOCKADDR_STORAGE result{};
	SOCKADDR_STORAGE* sockaddr_ptr = std::addressof(result);

	switch (std::move(endpoint).GetAddressFamily())
	{
		case net::IpAddressFamily::IPv4:
		{
			IN_ADDR sk_addr{};
			if (not std::move(endpoint).GetIpAddress().TrySerialize(std::addressof(sk_addr.s_addr)))
			{
				break;
			}

			SOCKADDR_IN ipv4_addr
			{
				.sin_family = (std::uint16_t)net::IpAddressFamily::IPv4,
				.sin_port = std::move(endpoint).GetPort(),
				.sin_addr = std::move(sk_addr),
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv4_addr));
		}
		break;

		case net::IpAddressFamily::IPv6:
		{
			IN6_ADDR sk_addr{};
			if (not std::move(endpoint).GetIpAddress().TrySerialize(std::addressof(sk_addr.s6_addr)))
			{
				break;
			}

			SOCKADDR_IN6 ipv6_addr
			{
				.sin6_family = (std::uint16_t)net::IpAddressFamily::IPv6,
				.sin6_port = std::move(endpoint).GetPort(),
				.sin6_flowinfo = 0,
				.sin6_addr = std::move(sk_addr),
				.sin6_scope_id = 0,
			};

			result = *reinterpret_cast<const SOCKADDR_STORAGE*>(std::addressof(ipv6_addr));
		}
		break;

		case net::IpAddressFamily::Unknown:
		{}
		break;
	}

	return result;
}
