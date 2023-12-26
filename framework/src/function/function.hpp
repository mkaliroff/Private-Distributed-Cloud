#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <memory>

namespace pdc
{
template<typename>
class Function;

template<typename RET>
class Function<RET(void)>
{
private:
    class IFunction;
public:
    template<typename ARGUMENT>
    explicit Function(RET (*func)(ARGUMENT), ARGUMENT arg);
    template<typename ARGUMENT, typename OBJ>
    explicit Function(RET (OBJ::*func)(ARGUMENT), OBJ *obj,ARGUMENT arg);
    
    RET operator()();

private:
    std::shared_ptr<IFunction> m_ifunc;

    class IFunction
    {
    public:
        ~IFunction();
        virtual RET operator()() = 0;
    };

    template<typename ARGUMENT>
    class Freefunc : public IFunction
    {
    public:
        Freefunc(RET (*foo)(ARGUMENT), ARGUMENT arg);
        RET operator()();
    private:
        ARGUMENT m_arg;
        RET (*m_foo)(ARGUMENT);
    };

    template<typename OBJ, typename ARGUMENT>
    class Memberfunc : public IFunction
    {
    public:
        Memberfunc(RET (OBJ::*foo)(ARGUMENT), OBJ *obj, ARGUMENT arg);
        RET operator()();

    private:
        ARGUMENT m_arg;
        RET (OBJ::*m_foo)(ARGUMENT);
        OBJ *m_obj;

    };
};
/*********************************************************************/

template <typename RET>
template <typename ARGUMENT>
Function<RET(void)>::Function(RET (*func)(ARGUMENT), ARGUMENT arg) : m_ifunc(new Freefunc<ARGUMENT>(func, arg))
{
    // empty
}

template <typename RET>
template <typename ARGUMENT, typename OBJ>
Function<RET(void)>::Function(RET (OBJ::*func)(ARGUMENT), OBJ *obj, ARGUMENT arg) : m_ifunc(new Memberfunc<OBJ, ARGUMENT>(func, obj, arg))
{
    //empty
}

template <typename RET>
RET Function<RET(void)>::operator()(void)
{
    return (*m_ifunc)();
}

/*********************************************************************/

template <typename RET>
Function<RET(void)>::IFunction::~IFunction()
{
    //empty
}

template <typename RET>
template <typename OBJ, typename ARGUMENT>
Function<RET(void)>::Memberfunc<OBJ, ARGUMENT>::Memberfunc(RET (OBJ::*foo)(ARGUMENT), OBJ *obj, ARGUMENT arg)
    : m_arg(arg), m_foo(foo), m_obj(obj)
{
    //empty
}

template <typename RET>
template <typename OBJ, typename ARGUMENT>
inline RET Function<RET(void)>::Memberfunc<OBJ, ARGUMENT>::operator()()
{
    return (m_obj->*m_foo)(m_arg);
}

template <typename RET>
template <typename ARGUMENT>
inline Function<RET(void)>::Freefunc<ARGUMENT>::Freefunc(RET (*foo)(ARGUMENT), ARGUMENT arg)
    : m_arg(arg), m_foo(foo)
{
    //empty
}

template <typename RET>
template <typename ARGUMENT>
inline RET Function<RET(void)>::Freefunc<ARGUMENT>::operator()()
{
    return (*m_foo)(m_arg);
}

} // pdc

#endif /* FUNCTION_HPP*/

