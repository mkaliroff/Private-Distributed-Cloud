// #ifndef ILRD_RD141_DISPATCHER_HPP
// #define ILRD_RD141_DISPATCHER_HPP

// #include <functional> // function
// #include <vector> // vector
// #include <algorithm> // find

// namespace ilrd_rd141
// {
// template<typename... ArgTypes>
// class Callback;

// template<typename... ArgTypes>
// class Dispatcher
// {
// public:
//     Dispatcher();
//     ~Dispatcher() noexcept;
//     Dispatcher(const Dispatcher &other) = delete;
//     Dispatcher(Dispatcher &&other) = delete;
//     Dispatcher &operator=(const Dispatcher &other) = delete;
//     Dispatcher &operator=(Dispatcher &&other) = delete;
//     void Notify(ArgTypes... args);
// private:
//     void Subscribe(const Callback<ArgTypes...> &callback);
//     void UnSubscribe(const Callback<ArgTypes...> &callback);
//     std::vector<Callback<ArgTypes... > *> m_callbacks;
//     friend Callback<ArgTypes...>;
// };

// template<typename... ArgTypes>
// class Callback
// {
// public:
//     Callback(const std::function<void(ArgTypes...)> &toDoOnNotify,
//     const std::function<void(void)> &OnDispatcherDeath);
//     ~Callback() noexcept;
//     Callback(const Callback &other) = delete;
//     Callback(Callback &&other) = delete;
//     Callback &operator=(const Callback &other) = delete;
//     Callback &operator=(Callback &&other) = delete;
//     void SetDispatcher(Dispatcher<ArgTypes...> *dispatcher);

// private:
//     bool operator==(const Callback &other) const;
//     void operator()(ArgTypes... args);
//     std::function<void(ArgTypes...)> m_toDoOnNotify;
//     std::function<void(void)> m_onDispatcherDeath;
//     Dispatcher<ArgTypes...> *m_dispatcher;
//     bool m_dispatcherDeath;
//     void OnDispatcherDeath();
//     friend Dispatcher<ArgTypes...>;
// };

// /******************************************************************************/

// template <typename... ArgTypes>
// Dispatcher<ArgTypes...>::Dispatcher()
// {
//     //empty
// }

// template <typename... ArgTypes>
// Dispatcher<ArgTypes...>::~Dispatcher() noexcept
// {
//     for(auto &subscriber : m_callbacks)
//     {
//         subscriber->OnDispatcherDeath();
//         subscriber->m_dispatcherDead = true;
//     }
// }

// template <typename... ArgTypes>
// void Dispatcher<ArgTypes...>::Notify(ArgTypes... args)
// {
//     for(auto &subscriber : m_callbacks)
//     {
//         (*subscriber)(args...);
//     }
// }

// template <typename... ArgTypes>
// void Dispatcher<ArgTypes...>::Subscribe(const Callback<ArgTypes...> &callback)
// {
//     m_callbacks.push_back(&(const_cast<Callback<ArgTypes...> &>(callback)));
// }

// template <typename... ArgTypes>
// void Dispatcher<ArgTypes...>::UnSubscribe(const Callback<ArgTypes...> &callback)
// {
//     auto it = find(m_callbacks.begin(), m_callbacks.end(), &callback);
//     if(it != m_callbacks.end())
//     {
//         m_callbacks.erase(it);
//     }
// }

// /******************************************************************************/

// template <typename... ArgTypes>
// Callback<ArgTypes...>::Callback(const std::function<void(ArgTypes...)> &toDoOnNotify,
// const std::function<void(void)> &onDispatcherDeath) :  m_toDoOnNotify(toDoOnNotify),
//  m_onDispatcherDeath(onDispatcherDeath), m_dispatcherDead(false)
// {
//     //empty
// }

// template <typename... ArgTypes>
// Callback<ArgTypes...>::~Callback() noexcept
// {
//     if(!m_dispatcherDead)
//     {
//         m_dispatcher->UnSubscribe(*this);
//     }
// }

// template <typename... ArgTypes>
// bool Callback<ArgTypes...>::operator==(const Callback &other) const
// {
//     return (other == *this);
// }

// template <typename... ArgTypes>
// void Callback<ArgTypes...>::operator()(ArgTypes... args)
// {
//     m_toDoOnNotify(args...);
// }

// /******************************************************************************/

// template <typename... ArgTypes>
// void Callback<ArgTypes...>::SetDispatcher(Dispatcher<ArgTypes...> *dispatcher)
// {
//     if(nullptr != dispatcher)
//     {
//         dispatcher->UnSubscribe(*this);
//     }
//     dispatcher->Subscribe(*this);
// }

// template <typename... ArgTypes>
// void Callback<ArgTypes...>::OnDispatcherDeath()
// {
//     subscriber->m_onDispatcherDeath();
//     m_dispatcher = nullptr;

// }

// } // ilrd_rd141

// #endif /* ILRD_RD141_DISPATCHER_HPP*/


#ifndef ILRD_RD141_DISPATCHER_HPP
#define ILRD_RD141_DISPATCHER_HPP

#include <functional> // function
#include <deque> // deque
#include <algorithm>

namespace ilrd_rd141
{
template<typename... ArgTypes>
class Callback;

template<typename... ArgTypes>
class Dispatcher //user cannot kill the dispatcher in callback
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
    std::deque<Callback<ArgTypes... > *> m_subscribers;
    friend Callback<ArgTypes...>;
};

template<typename... ArgTypes>
class Callback
{
public:
    Callback(const std::function<void(ArgTypes...)> &toDoOnNotify,
    const std::function<void(void)> &OnDispatcherDeath);
    ~Callback() noexcept;
    Callback(const Callback &other) = delete;
    Callback(Callback &&other) = delete;
    Callback &operator=(const Callback &other) = delete;
    Callback &operator=(Callback &&other) = delete;

    void SetDispatcher(Dispatcher<ArgTypes...> &dispatcher);

private:
    bool operator==(const Callback &other) const;
    void operator()(ArgTypes... args);
    Dispatcher<ArgTypes...> *m_dispatcher;
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
        subscriber->m_dispatcherDead = true;
        subscriber->m_onDispatcherDeath();
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
Callback<ArgTypes...>::Callback(const std::function<void(ArgTypes...)> &toDoOnNotify, const std::function<void(void)> &onDispatcherDeath)
: m_toDoOnNotify(toDoOnNotify), m_onDispatcherDeath(onDispatcherDeath), m_dispatcherDead(false)
{
    // empty
}

template <typename... ArgTypes>
Callback<ArgTypes...>::~Callback() noexcept
{
    if(!m_dispatcherDead)
    {
        m_dispatcher->UnSubscribe(*this);
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

template <typename... ArgTypes>
void Callback<ArgTypes...>::SetDispatcher(Dispatcher<ArgTypes...> &dispatcher)
{
    m_dispatcher = &dispatcher;
    if(!m_dispatcherDead)
    {
        m_dispatcher->Subscribe(*this);
    }
}

} // ilrd_rd141

#endif /* ILRD_RD141_DISPATCHER_HPP*/