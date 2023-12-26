#include "dll_loader.hpp"
#include <dlfcn.h>
#include <stdexcept>
#include <iostream>

namespace pdc
{
DllLoader::~DllLoader()
{
    for(auto &it : m_handles)
    {
        dlclose(it.second);
    }
}

void DllLoader::Load(const std::string &path)
{
    void *handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle)
    {
        throw std::runtime_error(dlerror());
    }
    m_handles[path] = handle;
}

void DllLoader::Unload(const std::string &path)
{
    if(m_handles.end() != m_handles.find(path))
    {
        dlclose(m_handles[path]);
        m_handles.erase(path);
    }
}
} // namespace pdc
