#include <unistd.h>

#include "logger.hpp"

int main()
{

    using namespace pdc;

    Logger& logger = Singleton<Logger>::GetInstance();

    logger.Log("Captain's log, Stardate 4523.3.");
    sleep(1);
    logger.Log("Deep Space Station K-7 has issued a priority one call.");
    sleep(1);
    logger.Log("More than an emergency, it signals near or total disaster.");
    sleep(1);
    logger.Log("We can only assume the Klingons have attacked the station.");
    sleep(1);
    logger.Log("We're going in armed for battle.");

    return 0;
}
