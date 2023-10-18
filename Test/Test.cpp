#pragma comment(lib, "Modern Network.lib")
#include <print>
#include <exception>
#include <coroutine>
#include <thread>
#include <future>

import Net.Constraints;
import Net.Application;
import Net.IpAddress;
import Net.IpAddress.IPv4;
import Net.Socket;
import Net.Task;
// A program-defined type on which the coroutine_traits specializations below depend
struct as_coroutine {};

// Enable the use of std::future<T> as a coroutine type
// by using a std::promise<T> as the promise type.
template <typename T, typename... Args>
	requires(!std::is_void_v<T> && !std::is_reference_v<T>)
struct std::coroutine_traits<std::future<T>, as_coroutine, Args...>
{
	struct promise_type : std::promise<T>
	{
		std::future<T> get_return_object() noexcept
		{
			return this->get_future();
		}

		std::suspend_never initial_suspend() const noexcept { return {}; }
		std::suspend_never final_suspend() const noexcept { return {}; }

		void return_value(const T& value)
			noexcept(std::is_nothrow_copy_constructible_v<T>)
		{
			this->set_value(value);
		}

		void return_value(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
		{
			this->set_value(std::move(value));
		}

		void unhandled_exception() noexcept
		{
			this->set_exception(std::current_exception());
		}
	};
};

// Same for std::future<void>.
template <typename... Args>
struct std::coroutine_traits<std::future<void>, as_coroutine, Args...>
{
	struct promise_type : std::promise<void>
	{
		std::future<void> get_return_object() noexcept
		{
			return this->get_future();
		}

		std::suspend_never initial_suspend() const noexcept { return {}; }
		std::suspend_never final_suspend() const noexcept { return {}; }

		void return_void() noexcept
		{
			this->set_value();
		}

		void unhandled_exception() noexcept
		{
			this->set_exception(std::current_exception());
		}
	};
};

// Allow co_await'ing std::future<T> and std::future<void>
// by naively spawning a new thread for each co_await.
template <typename T>
auto operator co_await(std::future<T> future) noexcept
	requires(!std::is_reference_v<T>)
{
	struct awaiter : std::future<T>
	{
		bool await_ready() const noexcept
		{
			using namespace std::chrono_literals;
			return this->wait_for(0s) != std::future_status::timeout;
		}

		void await_suspend(std::coroutine_handle<> cont) const
		{
			std::thread([this, cont] {
				this->wait();
				cont();
			}).detach();
		}

		T await_resume() { return this->get(); }
	};

	return awaiter{ std::move(future) };
}

// Utilize the infrastructure we have established.
std::future<int> compute(as_coroutine)
{
	int a = co_await std::async([] { return 6; });
	int b = co_await std::async([] { return 7; });
	co_return a * b;
}

std::future<void> fail(as_coroutine)
{
	throw std::runtime_error("bleah");
	co_return;
}

void Worker()
{
	net::Socket test_socket1 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	auto binded = test_socket1.Bind(net::IPv4Address::Loopback, 52001);
	if (binded.has_value())
	{
		std::println("binded! (1)");
	}

	net::Socket test_socket2 = net::Socket::Create(net::InternetProtocols::TCP, net::IpAddressFamily::IPv4);

	binded = test_socket2.Bind(net::IPv4Address::Loopback, 52002);
	if (binded.has_value())
	{
		std::println("binded! (2)");
	}

	auto connected = test_socket1.Connect(net::IPv4Address::Loopback, 52000);
	if (connected.has_value())
	{
		std::println("connected! (1)");
	}

	auto connector = test_socket2.ConnectAsync(net::IPv4Address::Loopback, 52000);
	connector();

	auto b = connector.Result();

	if (b.has_value())
	{
		std::println("connected! (2)");
	}

	std::println("Worker done!");
}

int main()
{
	std::println("=========== Init ===========");
	net::Application app{};

	std::println("=========== Awake ===========");
	app.Awake();

	std::println("=========== Start ===========");
	app.Start();

	Worker();

	std::println("=========== Update ===========");
	while (true)
	{
		app.Update(0);
	}
}
