module Net.SocketPool;
import <algorithm>;

net::SocketPool::SocketPool(const size_t& size)
	: myStation(), myPool()
{
	auto station = io::Station::Create();
	myStation = std::exchange(station.value(), {});

	myPool.reserve(size);
}

net::SocketPool::~SocketPool()
{
	std::ranges::for_each(myPool, [](EncapsuledSocket& ck) {
		static_cast<Socket*&>(ck.sk)->Close();
	});

	myStation.Destroy();

	myPool.clear();
}

void
net::SocketPool::Add(net::Socket* const& socket_ptr, std::uint64_t id)
{
	if (myStation.Register(*socket_ptr, id))
	{
		myPool.push_back(EncapsuledSocket{ socket_ptr, id });
		std::sort_heap(myPool.begin(), myPool.end());
	}
}

bool
net::SocketPool::TryAdd(net::Socket* const& socket_ptr, std::uint64_t id)
noexcept
{
	if (myStation.Register(*socket_ptr, id))
	{
		try
		{
			myPool.push_back(EncapsuledSocket{ socket_ptr, id });
			std::sort_heap(myPool.begin(), myPool.end());
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

	return false;
}

net::Socket* const
net::SocketPool::Allocate(std::uint64_t id, SocketType type, const InternetProtocols& protocol, const IpAddressFamily& family)
{
	auto sk_result = Socket::Create(type, protocol, family);
	if (sk_result.IsAvailable())
	{
		auto& ck = myPool.emplace_back();
		*ck.sk = std::exchange(sk_result, {});
		ck.id = id;

		return ck.sk;
	}

	return nullptr;
}

net::SocketPool::data_t::iterator
net::SocketPool::Find(std::uint64_t id)
noexcept
{
	return std::ranges::find(myPool, id
		, [](const EncapsuledSocket& ck) noexcept { return ck.id; }
	);
}

net::SocketPool::data_t::const_iterator
net::SocketPool::Find(std::uint64_t id)
const noexcept
{
	return std::ranges::find(myPool, id
		, [](const EncapsuledSocket& ck) noexcept { return ck.id; }
	);
}
