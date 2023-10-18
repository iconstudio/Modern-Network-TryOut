export module Net.Coroutine.Awaitable;

export namespace net
{
	template<typename T>
	concept Awaitable = requires(T object)
	{
		co_await object;
	};
}
