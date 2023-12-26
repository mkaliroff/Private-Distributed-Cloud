#include <iostream>
#include <cassert>

#include "function.hpp"

class Fun
{
public:
    Fun(int num) : m_num(num)
    {
        //empty
    }
    int GetNum()
    {
        return m_num;
    }
    int SetNum(int num) 
    {
        m_num = num;
        return m_num;
    }
private:
    int m_num;
};

int MulFive(int num)
{
    return 5 * num;
}

int main()
{
    using namespace pdc;

    Fun fun(10);

    Function<int(void)> f1(&Fun::SetNum, &fun, 8);
    Function<int(void)> f2(&MulFive, 10);

    std::cout << f1() << std::endl;
    std::cout << f2() << std::endl;

    std::cout << "all test passed" <<std::endl;
    return 0;
}