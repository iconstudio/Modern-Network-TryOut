module;

module Net.Task;
import <thread>;

template<net::notvoids T>
bool
net::Task<T>::TaskEngine::await_ready()
const noexcept
{
	return true;
}

template<net::notvoids T>
void
net::Task<T>::TaskEngine::await_suspend(net::Task<T>::handle_type handle)
const
{
	std::thread([this, &handle] {
		valueHandle.wait();
		handle();
	}).detach();
}

template<net::notvoids T>
T net::Task<T>::TaskEngine::await_resume()
{
	return valueHandle.get();
}
