export module Net.InternalProtocols;

export namespace net
{
	enum class [[nodiscard]] InternalProtocols
	{
		Unknown, TCP, UDP, RDP, BTH
	};
}
