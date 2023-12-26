#ifndef DIRMONITOR_HPP
#define DIRMONITOR_HPP

#include <functional>//function
#include <string>//string
#include <thread> //thread

#include "dispatcher.hpp" //Dispatcher

namespace pdc
{
class DirMonitor
{
public:
    enum WhatToMonitor{creation, deletion, modify};

    explicit DirMonitor(const std::string &path);
    ~DirMonitor() noexcept;
    DirMonitor(const DirMonitor &other) = delete;
    DirMonitor(DirMonitor &&other) = delete;
    DirMonitor &operator=(const DirMonitor &other) = delete;
    DirMonitor &operator=(DirMonitor &&other) = delete;

    void Subscribe(Callback<std::string> &toDoOnNotify, WhatToMonitor flag);
    private:
    int m_fd;
    bool m_time_to_exit;
    std::thread m_thread;
    void Worker();
    Dispatcher<std::string>  m_dis_creation;
    Dispatcher<std::string>  m_dis_deletion;
    Dispatcher<std::string>  m_dis_modify;

};

} // pdc

#endif /* DIRMONITOR_HPP*/