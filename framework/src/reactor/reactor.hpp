#ifndef REACTOR_HPP__
#define REACTOR_HPP__

#include <unordered_map>          // std::unordered_map

#include "function.hpp"           // Function<void(void)> callback API

namespace pdc
{
using Handler = pdc::Function<void(void)>;
template<typename Monitor> 
class Reactor
{
public:
    Reactor(Monitor *monitor_sptr);
    Reactor(const Reactor &) = delete;
    Reactor & operator=(const Reactor &) = delete;
    ~Reactor();
    void RegisterReadEventHandler(int fd, const Handler &call_back);
    void DeregisterEventHandler(int fd) noexcept;
    void Run(void);

private:
    std::shared_ptr<Monitor> m_monitor;
    std::unordered_map<int, Handler> m_call_backs;
};

template <typename Monitor>
inline Reactor<Monitor>::Reactor(Monitor *monitor_sptr) : m_monitor(monitor_sptr)
{
    //empty
}

template <typename Monitor>
inline Reactor<Monitor>::~Reactor()
{
    //empty
}

template <typename Monitor>
inline void Reactor<Monitor>::RegisterReadEventHandler(int fd, const Handler &call_back)
{
    m_call_backs.insert({fd, call_back});
    m_monitor->Monitor::RegisterRead(fd);
}

template <typename Monitor>
inline void Reactor<Monitor>::DeregisterEventHandler(int fd) noexcept
{
    m_call_backs.erase(fd);
    m_monitor->Monitor::Deregister(fd);
}


template <typename Monitor>
inline void Reactor<Monitor>::Run(void)
{
    while(true)
    {
        int fd_count = m_monitor->Monitor::WaitForEvents();

        for(int i = 0 ; i < fd_count ; i++)
        {
            m_call_backs.at(m_monitor->Monitor::ToNext())();

        }
    }
}



} // pdc

#endif /* REACTOR_H__ */
