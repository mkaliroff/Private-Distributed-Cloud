#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <unordered_map>//unordered_map
#include <memory>//shared_ptr
#include "singleton.hpp"//

namespace pdc
{
namespace Factory
{
class DoubleAdd : public std::exception
{
    const char*what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;
};


template<class Base,typename Key, typename... Types>
class Factory
{
public:
    Factory(const Factory &other) = delete;
    Factory &operator=(const Factory &lhs) = delete;
    //shared_ptr don't have to get a new pointer, you can choose a Deleter 
    //Add throw DoubleAdd if key is already in use
    void Add(const Key &key, std::shared_ptr<Base>(*creator)(Types...));
    //overrides if key is already in use
    void Set(const Key &key, std::shared_ptr<Base>(*creator)(Types...));
    std::shared_ptr<Base> Create(const Key &key,Types... args) const;
private:
    Factory() = default;
    std::unordered_map<Key, std::shared_ptr<Base>(*)(Types...)> m_creators;
    friend Singleton<Factory>;
};

template <class Base, typename Key, typename... Types>
void Factory<Base, Key, Types...>::Add(const Key &key, std::shared_ptr<Base> (*creator)(Types...))
{
    if(m_creators.end() != find(key))
    {
        throw DoubleAdd();
    }
    m_creators[key] = creator;
}

template <class Base, typename Key, typename... Types>
void Factory<Base, Key, Types...>::Set(const Key &key, std::shared_ptr<Base> (*creator)(Types...))
{
    m_creators[key] = creator;
}

template <class Base, typename Key, typename... Types>
std::shared_ptr<Base> Factory<Base, Key, Types...>::Create(const Key &key, Types... args) const
{
    return m_creators.at(key)(args...);
}

const char* DoubleAdd::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
{
    return "DoubleAdd";
}

}
} // pdc

#endif // FACTORY_HPP