#include <iostream>

#include "dir_monitor.hpp"

using namespace pdc;

void onCreation(const std::string& filename)
{
    std::cout << "File created: " << filename << std::endl;
}

void onDeletion(const std::string& filename)
{
    std::cout << "File deleted: " << filename << std::endl;
}

void onModify(const std::string& filename)
{
    std::cout << "File modified: " << filename << std::endl;
}

int main()
{
    pdc::DirMonitor dirMonitor(".");

    pdc::Callback<std::string> creationCallback(onCreation, [](){});
    pdc::Callback<std::string> deletionCallback(onDeletion, [](){});
    pdc::Callback<std::string> modifyCallback(onModify, [](){});

    dirMonitor.Subscribe(creationCallback, pdc::DirMonitor::creation);
    dirMonitor.Subscribe(deletionCallback, pdc::DirMonitor::deletion);
    dirMonitor.Subscribe(modifyCallback, pdc::DirMonitor::modify);

    std::this_thread::sleep_for(std::chrono::seconds(20));

    return 0;
}
