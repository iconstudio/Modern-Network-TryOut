module Net.Coroutine;
import <thread>;

void
net::coroutine::Coroutine::Start()
const
{
	if (myHandle and not isTriggered)
	{
		isTriggered = true;

		myHandle();
	}
}

void
net::coroutine::Coroutine::StartAsync()
const
{
	if (myHandle and not isTriggered)
	{
		isTriggered = true;

		std::thread{
			[handle = myHandle] {
				handle();
			}
		}.detach();
	}
}

void
net::coroutine::Coroutine::Resume()
const
{
	if (myHandle and isTriggered)
	{
		myHandle();
	}
}
void
net::coroutine::Coroutine::operator()()
const
{
	if (myHandle and isTriggered)
	{
		myHandle();
	}
}
