#include "MySocket.h"
#include "MyChannel.h"
#include "MyEventLoop.h"
#include "MyServer.h"

#include <sys/resource.h>

int main()
{
    // test
    struct rlimit rl;
    getrlimit(RLIMIT_NOFILE, &rl);
    printf("rlimit: soft=%ld hard=%ld\n", rl.rlim_cur, rl.rlim_max);

    MyEventLoop *loop = new MyEventLoop();
    MyServer *server = new MyServer(loop);
    loop->startLoop();
    return 0;
}
