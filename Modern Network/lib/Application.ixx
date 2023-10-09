export module Net.Application;
import Net.IObject;

export namespace net
{
	class Application : public IObject
	{
	public:
		constexpr Application() noexcept;
		~Application() noexcept override;

		void Awake() override;
		void Start() override;
		void Update(float) override;

	private:
	};
}
