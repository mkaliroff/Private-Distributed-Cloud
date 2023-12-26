#include <iostream>
#include <chrono>
#include "thread_pool.hpp"
#include "callable.hpp"

using namespace pdc;


void FreeFunc(int stab)
{
    (void)stab;
    std::cout << "Func" << std::endl;
}

int main()
{
    pdc::ThreadPool threadPool(0, 4);

    std::shared_ptr<Callable> task = std::make_shared<CallableFunction>(FreeFunc, 0);
    for (int i = 0; i < 8; ++i)
    {
        threadPool.AddTask(task, ThreadPool::Priority::medium);
    }

    threadPool.SetNumOfThreads(8);


    std::cout << "Run" << std::endl;

    threadPool.Run();


    threadPool.Pause();


    threadPool.SetNumOfThreads(4);



    threadPool.SetNumOfThreads(3);

    threadPool.Pause();

    threadPool.Run();


    return 0;
