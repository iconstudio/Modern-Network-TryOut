export module Net.SocketPool;
import Net.Socket;
import Net.Io.Station;
import <cstdint>;
import <vector>;
import <array>;
import <span>;
import <algorithm>;

export namespace net
{
	struct EncapsuledSocket
	{
		[[nodiscard]]
		static inline constexpr friend bool operator<(const EncapsuledSocket& lhs, const EncapsuledSocket& rhs) noexcept
		{
			return lhs.id < rhs.id;
		}

		Socket* const sk;
		std::uint64_t id;
	};

	class SocketPool
	{
	public:
		using data_t = std::vector<EncapsuledSocket>;
		using span_t = std::span<EncapsuledSocket>;
		using const_span_t = std::span<const EncapsuledSocket>;

		SocketPool(const size_t& size);
		~SocketPool();

		void Add(Socket* const& socket_ptr, std::uint64_t id)
		{
			if (myStation.Register(*socket_ptr, id))
			{
				myPool.push_back(EncapsuledSocket{ socket_ptr, id });
				std::ranges::sort_heap(myPool);
			}
		}

		[[nodiscard]]
		bool TryAdd(Socket* const& socket_ptr, std::uint64_t id) noexcept
		{
			if (myStation.Register(*socket_ptr, id))
			{
				try
				{
					myPool.push_back(EncapsuledSocket{ socket_ptr, id });
					std::ranges::sort_heap(myPool);
				}
				catch (...)
				{
					return false;
				}

				return true;
			}

			return false;
		}

		Socket* const Allocate(std::uint64_t id);

		[[nodiscard]]
		constexpr data_t::iterator Find(std::uint64_t id) noexcept
		{
			return std::ranges::find(myPool);
		}

		[[nodiscard]]
		constexpr data_t::const_iterator Find(std::uint64_t id) const noexcept
		{
			return std::ranges::find(myPool);
		}

		[[nodiscard]]
		constexpr span_t Subrange(const size_t& count) noexcept
		{
			return span_t{ myPool.begin(), count };
		}

		[[nodiscard]]
		constexpr const_span_t Subrange(const size_t& count) const noexcept
		{
			return const_span_t{ myPool.begin(), count };
		}

		[[nodiscard]]
		constexpr span_t Subrange(const size_t& first_index, const size_t& count) noexcept
		{
			return span_t{ myPool.begin() + first_index, count };
		}

		[[nodiscard]]
		constexpr const_span_t Subrange(const size_t& first_index, const size_t& count) const noexcept
		{
			return const_span_t{ myPool.begin() + first_index, count };
		}

		[[nodiscard]]
		constexpr data_t::iterator begin() noexcept
		{
			return myPool.begin();
		}

		[[nodiscard]]
		constexpr data_t::iterator end() noexcept
		{
			return myPool.end();
		}

		[[nodiscard]]
		constexpr data_t::const_iterator begin() const noexcept
		{
			return myPool.begin();
		}

		[[nodiscard]]
		constexpr data_t::const_iterator end() const noexcept
		{
			return myPool.end();
		}

		[[nodiscard]]
		constexpr data_t::const_iterator cbegin() const noexcept
		{
			return myPool.cbegin();
		}

		[[nodiscard]]
		constexpr data_t::const_iterator cend() const noexcept
		{
			return myPool.cend();
		}

		[[nodiscard]]
		constexpr data_t::size_type size() const noexcept
		{
			return myPool.size();
		}

		[[nodiscard]]
		constexpr data_t::size_type max_size() const noexcept
		{
			return myPool.max_size();
		}

	protected:
		io::Station myStation;

	private:
		SocketPool(const SocketPool&) = delete;
		SocketPool& operator=(const SocketPool&) = delete;

		data_t myPool;
	};
}
