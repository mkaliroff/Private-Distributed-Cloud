#ifndef IMONITOR_HPP__
#define IMONITOR_HPP__

namespace pdc
{
class IMonitor
{
public:
    virtual ~IMonitor() = default;
    virtual void RegisterRead(int fd) noexcept = 0;
    virtual bool Deregister(int fd) = 0;
    virtual int WaitForEvents() = 0;
    virtual bool IsReadable(int fd) = 0;
    virtual int ToNext() = 0;
};

class Select : IMonitor
{
public:
    Select();
    ~Select() = default;
    void RegisterRead(int fd) noexcept;
    bool Deregister(int fd);
    int WaitForEvents();
    bool IsReadable(int fd);
    int ToNext();
private:
    fd_set m_set;
    fd_set m_set_copy;
    int m_max;
};

} // pdc



#endif /* IMONITOR_HPP__ */
