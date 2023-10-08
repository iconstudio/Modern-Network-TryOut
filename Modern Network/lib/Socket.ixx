export module Net.Socket;
import Net.NativeHandle;

export namespace net
{
	class [[nodiscard]] Socket
	{
	public:


		[[nodiscard]] static Socket Create() noexcept;
		[[nodiscard]] static Socket Create(int& error_code) noexcept;

		constexpr Socket(Socket&&) noexcept = default;
		constexpr Socket& operator=(Socket&&) noexcept = default;

	private:
		Socket(const Socket&) = delete;
		Socket& operator=(const Socket&) = delete;

		NativeHandle nativeHandle;
	};
}
