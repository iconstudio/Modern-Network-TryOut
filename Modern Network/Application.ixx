export module Net.Application;

export namespace net
{
	class Application
	{
	public:
		constexpr Application() noexcept;
		~Application() noexcept;

		void Awake();
		void Start();
		void Update();

	private:
	};
}
