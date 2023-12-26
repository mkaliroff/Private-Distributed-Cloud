#ifndef ILRD_RD141_DISPATCHER_HPP
#define ILRD_RD141_DISPATCHER_HPP

#include <functional> // function
#include <vector> // vector
#include <algorithm>

namespace ilrd_rd141
{
template<typename... ArgTypes>
class Callback;

template<typename... ArgTypes>
class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher() noexcept;
    Dispatcher(const Dispatcher &other) = delete;
    Dispatcher(Dispatcher &&other) = delete;
    Dispatcher &operator=(const Dispatcher &other) = delete;
    Dispatcher &operator=(Dispatcher &&other) = delete;

    void Notify(ArgTypes... args); 

private:
    void Subscribe(const Callback<ArgTypes...> &callback);
    void UnSubscribe(const Callback<ArgTypes...> &callback);
    std::vector<Callback<ArgTypes... > *> m_subscribers;
    friend Callback<ArgTypes...>;
};

template<typename... ArgTypes>
class Callback
{
public:
    Callback(Dispatcher<ArgTypes...> &dispatcher, 
    const std::function<void(ArgTypes...)> &toDoOnNotify,
    const std::function<void(void)> &OnDispatcherDeath);
    ~Callback() noexcept;
    Callback(const Callback &other) = delete;
    Callback(Callback &&other) = delete;
    Callback &operator=(const Callback &other) = delete;
    Callback &operator=(Callback &&other) = delete;

private:
    bool operator==(const Callback &other) const;
    void OnDispatcherDeath();
    void operator()(ArgTypes... args);
    Dispatcher<ArgTypes...> &m_dispatcher;
    std::function<void(ArgTypes...)> m_toDoOnNotify;
    std::function<void(void)> m_onDispatcherDeath;
    bool m_dispatcherDead;
    friend Dispatcher<ArgTypes...>;
};

template <typename... ArgTypes>
Dispatcher<ArgTypes...>::Dispatcher()
{
    //empty
}

template <typename... ArgTypes>
Dispatcher<ArgTypes...>::~Dispatcher() noexcept
{
    for(auto &subscriber : m_subscribers)
    {
        subscriber->m_onDispatcherDeath();
        subscriber->m_dispatcherDead = true;
    }
}

template <typename... ArgTypes>
void Dispatcher<ArgTypes...>::Notify(ArgTypes... args)
{
    for(auto &subscriber : m_subscribers)
    {
        (*subscriber)(args...);
    }
}

template <typename... ArgTypes>
void Dispatcher<ArgTypes...>::Subscribe(const Callback<ArgTypes...> &callback)
{
    m_subscribers.push_back(&(const_cast<Callback<ArgTypes...> &>(callback)));
}

template <typename... ArgTypes>
void Dispatcher<ArgTypes...>::UnSubscribe(const Callback<ArgTypes...> &callback)
{
    auto it = find(m_subscribers.begin(), m_subscribers.end(), &callback);
    if(it != m_subscribers.end())
    {
        m_subscribers.erase(it);
    }
}

/************************************************************/

template <typename... ArgTypes>
Callback<ArgTypes...>::Callback(Dispatcher<ArgTypes...> &dispatcher, const std::function<void(ArgTypes...)> &toDoOnNotify, const std::function<void(void)> &onDispatcherDeath)
: m_dispatcher(dispatcher), m_toDoOnNotify(toDoOnNotify), m_onDispatcherDeath(onDispatcherDeath), m_dispatcherDead(false)
{
    dispatcher.Subscribe(*this);
}

template <typename... ArgTypes>
Callback<ArgTypes...>::~Callback() noexcept
{
    if(!m_dispatcherDead)
    {
        m_dispatcher.UnSubscribe(*this);
    }
}

template <typename... ArgTypes>
bool Callback<ArgTypes...>::operator==(const Callback &other) const
{
    return (other == *this);
}

template <typename... ArgTypes>
void Callback<ArgTypes...>::operator()(ArgTypes... args)
{
    m_toDoOnNotify(args...);
}

} // ilrd_rd141

#endif /* ILRD_RD141_DISPATCHER_HPP*/
