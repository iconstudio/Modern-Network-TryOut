export module Net.Io.Entity;
import Net.Socket;
import Net.Property;
import Net.Io.Context;

export namespace net::io
{
	class Entity
	{
	public:
		Entity() noexcept = default;
		~Entity() noexcept = default;

		ReadonlyProperty<Socket> myConnectum;
		Context myContext;
	};
}
