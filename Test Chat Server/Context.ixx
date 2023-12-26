export module Test.Context;
import <cstdint>;
import <atomic>;
import Net.Io.Context;
export import Test.IoOperation;

export namespace test
{
	class ExContext : public net::io::Context
	{
	public:
		using Context::Context;

		std::uintptr_t myID;
		std::atomic<IoOperation> myOperation;
	};
}
