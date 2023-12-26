#ifndef DLLLOADER_HPP
#define DLLLOADER_HPP

#include <functional>//function
#include <string>//string
#include <unordered_map>
namespace pdc
{
class DllLoader
{
public:
    DllLoader() = default;
    ~DllLoader();
    DllLoader& operator=(const DllLoader&) = delete;
    DllLoader(DllLoader &&other) = delete;
    DllLoader &operator=(DllLoader &&other) = delete;
    void Load(const std::string &path);
    void Unload(const std::string &path);


private:
    std::unordered_map<std::string, void*> m_handles;
};
} // pdc

#endif /* DLLLOADER_HPP*/
