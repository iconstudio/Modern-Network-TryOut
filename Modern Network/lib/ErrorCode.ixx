export module Net.ErrorCode;
export import :General;
export import :Open;
export import :Option;
export import :Sending;
export import :Receiving;
export import :Closing;
import <cstdint>;

export namespace net
{
	[[nodiscard]]
	ErrorCodes AcquireNetworkError() noexcept;
	[[nodiscard]]
	SocketOpenErrorCodes AcquireListeningError() noexcept;
	[[nodiscard]]
	SocketClosingErrorCodes AcquireClosingError() noexcept;
	[[nodiscard]]
	SendingErrorCodes AcquireSendingError() noexcept;
	[[nodiscard]]
	ReceivingErrorCodes AcquireReceivingError() noexcept;
	[[nodiscard]]
	SocketOptionErrorCodes AcquireSocketOptionError() noexcept;

	[[nodiscard]]
	constexpr bool operator==(const ErrorCodes& lhs, const SendingErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ErrorCodes& lhs, const ReceivingErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ErrorCodes& lhs, const SocketOpenErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ErrorCodes& lhs, const SocketClosingErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ErrorCodes& lhs, const SocketOptionErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const SendingErrorCodes& lhs, const ReceivingErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ReceivingErrorCodes& lhs, const SendingErrorCodes& rhs) noexcept
	{
		return static_cast<std::int32_t>(lhs) == static_cast<std::int32_t>(rhs);
	}
}
