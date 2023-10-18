export module Net.Coroutine.IAwaitable;
import <coroutine>;

export namespace net
{
	template<typename R = void, typename H = std::coroutine_handle<void>>
	class IAwaitable
	{
	public:
		virtual constexpr bool await_ready() noexcept = 0;
		virtual void await_suspend(H handle) const = 0;
		virtual constexpr R await_resume() const = 0;
	};
}
