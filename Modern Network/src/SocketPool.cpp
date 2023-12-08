module Net.SocketPool;

net::SocketPool::SocketPool(const size_t& size)
	: myStation(), myPool()
{
	auto station = io::Station::Create();
	myStation = station.value();

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
