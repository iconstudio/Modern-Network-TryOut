export module Net.Socket.ErrorCode;
export import :General;
export import :Sending;
export import :Receiving;
export import :Open;
export import :Closing;

export namespace net
{
	[[nodiscard]]
	SocketErrorCodes AcquireSocketError() noexcept;
	[[nodiscard]]
	SendingErrorCodes AcquireSendingError() noexcept;
	[[nodiscard]]
	ReceivingErrorCodes AcquireReceivingError() noexcept;
	[[nodiscard]]
	SocketOpenErrorCodes AcquireListeningError() noexcept;

	[[nodiscard]]
	constexpr bool operator==(const SocketErrorCodes& lhs, const SendingErrorCodes& rhs) noexcept
	{
		return static_cast<int>(lhs) == static_cast<int>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const SocketErrorCodes& lhs, const ReceivingErrorCodes& rhs) noexcept
	{
		return static_cast<int>(lhs) == static_cast<int>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const SocketErrorCodes& lhs, const SocketOpenErrorCodes& rhs) noexcept
	{
		return static_cast<int>(lhs) == static_cast<int>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const SendingErrorCodes& lhs, const ReceivingErrorCodes& rhs) noexcept
	{
		return static_cast<int>(lhs) == static_cast<int>(rhs);
	}

	[[nodiscard]]
	constexpr bool operator==(const ReceivingErrorCodes& lhs, const SendingErrorCodes& rhs) noexcept
	{
		return static_cast<int>(lhs) == static_cast<int>(rhs);
	}
}
