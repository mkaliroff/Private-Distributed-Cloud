#include <cassert>
#include <cstdlib>
#include <cstring>

#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "buse.h"
#include "reactor.hpp"
#include "imonitor.hpp"


#define MB (1 << 20)

static int xmp_read(void *buf, u_int32_t len, u_int64_t offset, char *data, size_t counter);
static int xmp_write(const void *buf, u_int32_t len, u_int64_t offset, char *data, size_t counter);

/*****************************************************************************/

class RequestHandler
{
public:
    RequestHandler(size_t size, int socket);
    ~RequestHandler();
    void Handle(int unused);
private:
    size_t m_counter;
    int m_socket;
    char *m_buffer;
};

/******************************************************************************/

int main()
{
using namespace pdc;

    size_t size = 512 * MB;


    int nbd_socket = NbdOpen("/dev/nbd0", size);

    Reactor<Select> reactor(new Select);

    RequestHandler handler(size, nbd_socket);

    Function<void(void)> func_handler(&RequestHandler::Handle, &handler, 0); // 0 is an unused value

    reactor.RegisterReadEventHandler(nbd_socket, func_handler);

    reactor.Run();

    return 0;
}
/******************************************************************************/
static int xmp_read(void *buf, u_int32_t len, u_int64_t offset, char *buffer, size_t counter)
{
    std::memcpy(buf, buffer + offset, len);
    std::cout << counter << "\tRead Request\t" << len << " bytes" << std::endl;
    return 0;
}
/******************************************************************************/
static int xmp_write(const void *buf, u_int32_t len, u_int64_t offset, char *buffer, size_t counter)
{
    std::memcpy(buffer + offset, buf, len);
    std::cout << counter << "\tWrite Request\t" << len << " bytes" << std::endl;
    return 0;
}
/******************************************************************************/
RequestHandler::RequestHandler(size_t size, int socket) : m_counter(0), m_socket(socket), m_buffer(new char[size])
{
    //empty
}
/******************************************************************************/
RequestHandler::~RequestHandler()
{
    delete[] m_buffer;
}
/******************************************************************************/
void RequestHandler::Handle(int unused)
{    
    (void)unused;
    buse_request_t *request = NbdGetRequest(m_socket);
    switch(request->type)
    {
        case BUSE_CMD_READ: 
        {
            xmp_read(request->data, request->len ,request->from, m_buffer, m_counter++);
            break;
        }
        case BUSE_CMD_WRITE: 
        {
            xmp_write(request->data, request->len ,request->from, m_buffer, m_counter++);
            break;
        }
        case BUSE_CMD_DISC: 
        {
            break;
        }
        case BUSE_CMD_FLUSH: 
        {
            break;
        }
        case BUSE_CMD_TRIM: 
        {
            break;
        }
        default:
        {
            break;
        }
    }
    NbdRequestDone(m_socket, request);
}
/******************************************************************************/