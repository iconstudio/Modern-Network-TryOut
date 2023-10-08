export module Net.Coroutine.Awaitable;
import Net.Coroutine.Promissory;

export namespace net
{
	template<typename T>
	concept Awaitable = requires(T object)
	{
		{ T::promise_type } -> net::Promissory;
		co_await object;
	};
}
