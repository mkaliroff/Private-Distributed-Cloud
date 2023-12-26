#ifndef LOGGER_HPP__
#define LOGGER_HPP__

#include <thread>// thread
#include <fstream> // files

#include "singleton.hpp"
#include "waitable_queue.hpp"

namespace pdc
{

class Logger
{
public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger &&other) = delete;
    Logger &operator=(Logger &&other) = delete;
    ~Logger();
    
    void Log(const std::string& str);     

private:
    Logger();
    friend Singleton<Logger>;
    std::ofstream m_file;
    bool m_is_done;
    std::thread m_thread;
    WaitableQueue <std::string> m_queue;
    void Worker();

};

extern template class pdc::Singleton<pdc::Logger>;

} //pdc

#endif  // LOGGER_HPP