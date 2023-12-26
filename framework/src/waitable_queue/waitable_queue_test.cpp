#include <iostream>
#include <thread>
#include <queue>

#include "waitable_queue.hpp"
#include "pq_to_q.hpp"
#include "function.hpp"
#include <chrono>               // std::nanoseconds


using namespace pdc;

void Producer(size_t num, WaitableQueue<int, FrontablePQ<int>> &queue);
void Consumer(size_t num, WaitableQueue<int, FrontablePQ<int>> &queue);

int main()
{
    WaitableQueue<int, FrontablePQ<int>> queue;

    std::thread producer(Producer, 1, std::ref(queue));


    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std:: cout << std::endl;

    std::thread consumer1(Consumer, 1, std::ref(queue));
    std::thread consumer2(Consumer, 2, std::ref(queue));
    // std::thread consumer3(Consumer, 3, std::ref(queue));

    producer.join();
    consumer1.join();
    consumer2.join();
    // consumer3.join();

    std::cout << "all tests passed" << std::endl;

    return 0;
}

void Producer(size_t num, WaitableQueue<int, FrontablePQ<int>> &queue)
{
    for (int i = 0; i < 10 ; ++i)
    {
        int data = i % 4;
        std::cout << "Producer " << num <<" : " << data << std::endl;
        queue.Push(data);
    }
}

void Consumer(size_t num, WaitableQueue<int, FrontablePQ<int>> &queue)
{   
    std::chrono::nanoseconds timeout(100000);
    for (int i = 0; i < 5; ++i)
    {
        int data = 0;
        queue.Pop(data, timeout);
        std::cout << "Consumer " << num <<" : " << data << std::endl;
    }
}