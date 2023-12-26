#include<iostream>

void __attribute__((constructor)) Print();

void Print()
{
    std::cout << "This is a function on a shared object" << std::endl;
}