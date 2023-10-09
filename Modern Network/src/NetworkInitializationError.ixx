export module Net.Exception.NetworkInitializationError;
import <exception>;

export namespace net
{
	class [[nodiscard]] NetworkInitializationError : public std::exception
	{
		NetworkInitializationError() noexcept
			: exception("Error on Network Initialization")
		{}

		NetworkInitializationError(const char* const& contents) noexcept
			: exception(contents)
		{}
	};
}
