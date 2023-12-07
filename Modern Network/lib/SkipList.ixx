export module Net.Collection.SkipList;
import Net.Constraints;
import <mutex>;

export namespace net::collections
{
	template<typename T>
	class SkipList;

	template<notvoids T>
	class SkipList<T>
	{
	private:
		class NODE_SK
		{
		public:
			NODE_SK(const int& v, const int& top)
				: value(v), top_level(top)
				, nexts()
				, isRemoved(false), fullyLinked(false)
			{
				for (auto i = 0; i <= MAX_LEVEL; ++i)
				{
					nexts[i] = nullptr;
				}
			}

			NODE_SK() : NODE_SK(-1, 0) {}

			void lock()
			{
				myMutex.lock();
			}

			void unlock()
			{
				myMutex.unlock();
			}

			int value;
			int top_level;

			std::recursive_mutex myMutex;
			NODE_SK* volatile nexts[MAX_LEVEL + 1];

			volatile bool isRemoved;
			volatile bool fullyLinked;
		};

	public:

	};
}
