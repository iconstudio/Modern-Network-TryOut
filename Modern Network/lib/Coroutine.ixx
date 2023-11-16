export module Net.Coroutine;
export import :TimedAwaiter;
import :BasicCoroutine;
import :BasicPromise;
import <atomic>;
export import <coroutine>;

export namespace net::coroutine
{
	using std::suspend_never;
	using std::suspend_always;
	using std::coroutine_handle;

	class Coroutine final
		: public BasicCoroutine<Coroutine, BasicPromise<Coroutine, suspend_never, suspend_always, void>>
	{
	public:
		using handle_type = BasicCoroutine::handle_type;

		constexpr Coroutine(const handle_type& handle) noexcept
			: BasicCoroutine(handle), isTriggered()
		{}

		constexpr Coroutine(handle_type&& handle) noexcept
			: BasicCoroutine(static_cast<handle_type&&>(handle)), isTriggered()
		{}

		~Coroutine() noexcept(noexcept(BasicCoroutine::~BasicCoroutine())) = default;

		void Start() const
		{
			if (myHandle)
			{
				isTriggered = true;

				myHandle();
			}
		}

		void StartAsync() const
		{
			if (myHandle)
			{
				isTriggered = true;

				myHandle();
			}
		}

		void Resume() const
		{
			if (myHandle and isTriggered)
			{
				myHandle();
			}
		}

		void operator()() const
		{
			if (myHandle)
			{
				isTriggered = true;

				myHandle();
			}
		}

		[[nodiscard]]
		bool IsTriggered() const noexcept
		{
			return isTriggered.load(std::memory_order_relaxed);
		}

		[[nodiscard]]
		constexpr bool operator==(const Coroutine&) const noexcept = default;

	private:
		mutable volatile std::atomic_bool isTriggered;
	};
}

export namespace net
{
	using Coroutine = coroutine::Coroutine;
}
