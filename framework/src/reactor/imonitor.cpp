#include <sys/select.h>
#include <iostream>

#include "imonitor.hpp"

#define MAX(a, b)((a > b) ? a : b)

namespace pdc
{
Select::Select() : m_set({0}), m_set_copy({0}), m_max(0)
{
    //empty
}

void Select::RegisterRead(int fd) noexcept
{
    FD_SET(fd, &m_set);
    m_max = MAX(m_max, fd);
}

bool Select::Deregister(int fd)
{
    FD_CLR(fd, &m_set);
    return true;
}

int Select::WaitForEvents()
{
    m_set_copy = m_set;
    int fd_count = select(m_max + 1, &m_set_copy, NULL, NULL, NULL);

    return fd_count;
}

bool Select::IsReadable(int fd)
{
    if(FD_ISSET(fd, &m_set_copy))
    {
        return true;
    }
    return false;
}

int Select::ToNext()
{
    for(int i = 0 ; i < m_max + 1 ; i++)
    {
        if(true == FD_ISSET(i, &m_set_copy))
        {
            FD_CLR(i, &m_set_copy);
            return i;
        }
    }

    return -1;
}

}// pdc