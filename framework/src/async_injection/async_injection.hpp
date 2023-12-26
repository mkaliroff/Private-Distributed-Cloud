#ifndef ASYNC_INJECTION_HPP
#define ASYNC_INJECTION_HPP

#include <chrono>//nanoseconds
#include <functional>//function<bool()>
#include <memory>//shared_ptr
#include "callable.hpp"//Callable

namespace pdc
{ 

class Asyncinjection
{

public:
    Asyncinjection(const Asyncinjection& other) = delete;
    Asyncinjection& operator = (const Asyncinjection& other) = delete;
    Asyncinjection(Asyncinjection &&other) = delete;
    Asyncinjection &operator=(Asyncinjection &&other) = delete;

    static void CreateInstance(std::function<bool()> functor, std::chrono::nanoseconds interval);  

private:   
    ~Asyncinjection() = default;
    Asyncinjection
    (std::function<bool()> functor, std::chrono::nanoseconds interval);
    static void Do(Asyncinjection *self);

    std::function<bool()> m_toDo;
    std::shared_ptr<Callable> m_task;
    std::chrono::nanoseconds m_interval;
};

} // pdc

#endif // ASYNC_INJECTION_HPP