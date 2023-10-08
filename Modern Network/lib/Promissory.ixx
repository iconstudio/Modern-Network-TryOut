export module Net.Coroutine.Promissory;
import Net.Coroutine.Suspender;

export namespace net
{
	template<typename P>
	concept Promissory = requires(P object)
	{
		object.get_return_object();
		{ object.initial_suspend() } -> net::Suspender;
		{ object.final_suspend() } -> net::Suspender;
	};
}
