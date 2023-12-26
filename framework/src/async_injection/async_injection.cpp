#include "scheduler.hpp"//Schedular
#include "callable_function.hpp"//Function<void(void)>
#include "async_injection.hpp"//header

namespace pdc
{
void Asyncinjection::CreateInstance
(std::function<bool()> functor, std::chrono::nanoseconds interval)
{
	new Asyncinjection(functor, interval);
}

Asyncinjection::Asyncinjection(std::function<bool()> functor, 
std::chrono::nanoseconds interval) : m_toDo(functor), 
m_task(new Function<void(void)>(Do, this)), m_interval(interval)
{
	Do(this);
}

void Asyncinjection::Do(Asyncinjection *self)
{
	if(self->m_toDo())
	{
		delete self;
	}
	else
	{
		Singleton<Scheduler>::GetInstance().schedule(self->m_task, self->m_interval);
	}
}
} // pdc