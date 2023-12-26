#include <iostream>

#include "dispatcher.hpp"

using namespace ilrd_rd141;

void Test1();
void Test2();
void Test3();

class ViewWindow
{
public:
    void Display(int data)
    {
        std::cout << "ViewWindow displays: " << data << std::endl;
    }
};

class DataModel
{
public:
    ilrd_rd141::Dispatcher<int> dispatcher;

    void Notify(int data)
    {
        dispatcher.Notify(data);
    }
};

class ControlWindow
{
public:
    void Update(int data)
    {
        std::cout << "ControlWindow updates with data: " << data << std::endl;
    }
};


int main()
{
    
    Test1();
    Test2();
    Test3();

    return 0;
}

void Test1()
{
    ViewWindow view1;
    DataModel model1;

    ilrd_rd141::Callback<int> callback1([&view1](int data) { view1.Display(data); }, []() {});

    model1.Notify(42);

    std::cout << "Test 1 complete.\n";
}

void Test2()
{

    DataModel model1;
    ViewWindow view2;

    ControlWindow control2;

    ilrd_rd141::Callback<int> callback2( [&view2](int data) { view2.Display(data); }, []() {});
    ilrd_rd141::Callback<int> callback3( [&control2](int data) { control2.Update(data); }, []() {});

    model1.Notify(42);

    std::cout << "Test 2 complete.\n";
}

void Test3()
{
    DataModel model1;
    ViewWindow view3;

    ControlWindow control3;

    ilrd_rd141::Callback<int> callback4([&view3](int data) { view3.Display(data); }, []() {});
    ilrd_rd141::Callback<int> callback5([&control3](int data) { control3.Update(data); }, []() {});

    model1.Notify(42);

    std::cout << "Test 3 complete.\n";
}