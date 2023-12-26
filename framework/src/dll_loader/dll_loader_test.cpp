#include <iostream>

#include "dll_loader.hpp"

int main()
{
    using namespace pdc;

    DllLoader loader;

    loader.Load("./shared_object.so");


    loader.Unload("./shared_object.so");

    std::cout << "Test ended" << std::endl;

    return 0;
}
