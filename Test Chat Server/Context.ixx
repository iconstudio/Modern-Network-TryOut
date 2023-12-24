export module Test.Context;
import <cstdint>;
import Net.Io.Context;
import Test.IoOperation;

export namespace test
{
	class ExContext : public net::io::Context
	{
	public:
		using Context::Context;

		std::uintptr_t myID;
		IoOperation myOperation;
	};
}
