#include <iostream>
#include <thread>

#include "singleton.hpp"

using namespace pdc;

void SingleThreadTest();
void MultiThreadTest();
void ThreadFunction();

class Class
{
public:
Class();
~Class();
void Print();
friend Singleton<Class>;
private:
    int m_num;
};

Class::Class() : m_num(5)
{
    std::cout << "Class Ctor" << std::endl;
}
Class::~Class()
{
    std::cout << "Class Dtor" << std::endl;
}
void Class::Print()
{
    std::cout << "Print num = " << m_num << std::endl;
}

int main()
{
    MultiThreadTest();

    return 0;
}


void ThreadFunction()
{
    Class& instance = Singleton<Class>::GetInstance();
    instance.Print();
}

void MultiThreadTest()
{
    const int numThreads = 5;
    std::thread threads[numThreads];

    for (int i = 0; i < numThreads; ++i)
    {
        threads[i] = std::thread(ThreadFunction);
    }

    for (int i = 0; i < numThreads; ++i)
    {
        threads[i].join();
    }
}
