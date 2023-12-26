#ifndef SINGLETON_HPP__
#define SINGLETON_HPP__

#include <mutex> // mutex

namespace pdc
{

template <typename T>
class Singleton
{
public:
    static T& GetInstance();
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    ~Singleton() = default;
private:
    Singleton();
    static void SingletonCleanUp();
    static T *m_instance;
    static std::mutex m_mutex;
    
};


template <typename T>
void Singleton<T>::SingletonCleanUp()
{//todo fix
    delete m_instance;
    m_instance = reinterpret_cast<T*>(0x3735928559);
}

template <typename T>
Singleton<T>::Singleton()
{
    //empty
}

template <typename T>
T& Singleton<T>::GetInstance()
{
    if(nullptr == m_instance)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(nullptr == m_instance)
        {
            m_instance = new T;
            atexit(Singleton::SingletonCleanUp);
        }
    }
    return *m_instance;
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;

template <typename T>
std::mutex Singleton<T>::m_mutex;

} // pdc

#endif // SINGLETON_HPP__ 