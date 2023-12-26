#ifndef CALLABLE_FUNCTION_HPP
#define CALLABLE_FUNCTION_HPP

#include "function.hpp"
#include "callable.hpp"

namespace pdc
{
template<>
class Function<void(void)> : public Callable
{
public:
	template<typename ARG>
	explicit Function(void (*foo)(ARG), ARG arg);
	template<typename CALLER, typename ARG>
	explicit Function(void (CALLER::*foo)(ARG), CALLER *caller, ARG arg);

	inline void operator()();

private:
	class IFunctor;

	std::shared_ptr<IFunctor> m_func;

	class IFunctor
	{
	public:
		inline virtual ~IFunctor();

		inline virtual void operator()() = 0;
	};
	
	template<typename ARG>
	class ArgFunctor : public IFunctor
	{
	public:
		explicit ArgFunctor(void (*foo)(ARG),ARG arg);

		void operator()();
	
	private:
		void(*m_func)(ARG) ;
		ARG m_arg;
	};

	template<typename CALLER, typename ARG>
	class MemberFunctor : public IFunctor
	{
	public:
		explicit MemberFunctor(void (CALLER::*foo)(ARG), CALLER *caller,ARG arg);

		void operator()();
	
	private:
		void (CALLER::*m_func)(ARG);
		CALLER *m_caller;
		ARG m_arg;
	};
};

template <typename ARG>
Function<void(void)>::Function(void (*foo)(ARG), ARG arg) : 
m_func(new ArgFunctor<ARG>(foo, arg))
{
	//empty
}

template <typename CALLER, typename ARG>
Function<void(void)>::Function(void (CALLER::*foo)(ARG), CALLER *caller, ARG arg) : 
m_func(new MemberFunctor<CALLER, ARG>(foo, caller, arg))
{
	//empty
}

void Function<void(void)>::operator()()
{
	return (*m_func)();
}

Function<void(void)>::IFunctor::~IFunctor()
{
	//empty
}

template <typename ARG>
Function<void(void)>::ArgFunctor<ARG>::ArgFunctor(void (*foo)(ARG), ARG arg) : 
m_func(foo), m_arg(arg)
{
	//empty
}

template <typename ARG>
void Function<void(void)>::ArgFunctor<ARG>::operator()()
{
	return m_func(m_arg);
}


template <typename CALLER, typename ARG>
Function<void(void)>::MemberFunctor<CALLER, ARG>::MemberFunctor
(void (CALLER::*foo)(ARG), CALLER *caller, ARG arg) : m_func(foo), m_caller(caller),
m_arg(arg)
{
	//empty
}

template <typename CALLER, typename ARG>
void Function<void(void)>::MemberFunctor<CALLER, ARG>::operator()()
{
	return (m_caller->*m_func)(m_arg);
}

} // pdc

#endif /* CALLABLE_FUNCTION_HPP*/
