export module Net.Application;
import Net.IObject;
import Net.Socket;

export namespace net
{
	class Application : public IObject
	{
	public:
		Application();
		~Application() noexcept override;

		void Awake() override;
		void Start() override;
		void Update(float) override;

		[[nodiscard]]
		static Socket& Listener() noexcept;

	private:
		Application(const Application&) = delete;
		void operator=(const Application&) = delete;
		Application(Application&&) = delete;
		void operator=(Application&&) = delete;
	};
}
