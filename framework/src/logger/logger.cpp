#include <ctime>
#include <chrono>
#include <iomanip>

#include "logger.hpp"
#include <iostream>
namespace pdc
{

static std::string GetTimeStamp();

Logger::Logger() : m_file(".log"), m_is_done(false), m_thread(&Logger::Worker, this)
{
    //empty
}

Logger::~Logger()
{
    m_is_done = true;
    m_thread.join();
    m_file.close();
}

void Logger::Log(const std::string &str)
{
   m_queue.Push(str);
}

void Logger::Worker()
{
    while(!m_is_done || !m_queue.IsEmpty())
    {
        std::string str;
        m_queue.Pop(str);
        m_file << GetTimeStamp() << " " << str <<std::endl;
    }
}

static std::string GetTimeStamp()
{
    auto now = std::chrono::system_clock::to_time_t((std::chrono::system_clock::now()));
    struct std::tm timeinfo;
    localtime_r(&now, &timeinfo);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%d/%m/%Y:%H:%M:%S", &timeinfo);
    std::string ret = timestamp;
    return ret;
}

} // pdc