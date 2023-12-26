#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>

#include "dir_monitor.hpp"

namespace pdc
{

DirMonitor::DirMonitor(const std::string &path) : m_fd(0), m_time_to_exit(false)
{
    m_fd = inotify_init();
    if (0 > m_fd)
    {
        perror("inotify_init");
        throw std::runtime_error("Failed to initialize inotify");
    }

    int wd = inotify_add_watch(m_fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
    if (0 > wd)
    {
        perror("inotify_add_watch");
        close(m_fd);
        throw std::runtime_error("Failed to add watch");
    }

    m_thread = std::thread(&DirMonitor::Worker, this);
}
/*****************************************************************************/
DirMonitor::~DirMonitor() noexcept
{
    m_time_to_exit = true;
    m_thread.join();
    close(m_fd);
}
/*****************************************************************************/
void DirMonitor::Subscribe(Callback<std::string> &toDoOnNotify, WhatToMonitor flag)
{
    switch (flag)
    {
    case creation:
        toDoOnNotify.SetDispatcher(m_dis_creation);
        break;
    case deletion:
        toDoOnNotify.SetDispatcher(m_dis_deletion);
        break;
    case modify:
        toDoOnNotify.SetDispatcher(m_dis_modify);
        break;    
    default:
        throw std::runtime_error("Invalid flag");
        break;
    }
}
/*****************************************************************************/
void DirMonitor::Worker()
{
    char buffer[4096];
    while(true != m_time_to_exit)
    {
        int len = read(m_fd, buffer, sizeof(buffer));

        struct inotify_event* event = reinterpret_cast<struct inotify_event*>(buffer);


        switch (event->mask)
        {
        case IN_CREATE:
            m_dis_creation.Notify(std::string(event->name));
            break;
        case IN_DELETE:
            m_dis_deletion.Notify(std::string(event->name));
            break;
        case IN_MODIFY:
            m_dis_modify.Notify(std::string(event->name));
            break;    
        default:
            throw std::runtime_error("Invalid mask");
            break;
        }
    }
}
/*****************************************************************************/
} // pdc