#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <memory>
#include "callable.hpp"
#include "singleton.hpp"

namespace pdc
{    


class Scheduler 
{
public:
    
    ~Scheduler();
    Scheduler(const Scheduler& other) = delete;
    Scheduler& operator = (const Scheduler& other) = delete;
    Scheduler(Scheduler &&other) = delete;
    Scheduler &operator=(Scheduler &&other) = delete;

    void schedule(std::shared_ptr<Callable> task, std::chrono::nanoseconds time_nano);

private:

    Scheduler();
    friend Singleton<Scheduler>;
    std::vector<timer_t> m_timerIds;

};
}

// extern template class pdc::Singleton<pdc::Scheduler>;

#endif  // SCHEDULER_HPP