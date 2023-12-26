
#include "async_injection.hpp"
#include "callable.hpp"
#include "scheduler.hpp"

using namespace pdc;

template class pdc::Singleton<pdc::Scheduler>;


int main()
{

    return 0;
}