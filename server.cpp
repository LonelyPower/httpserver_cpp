#include "src/MyEventLoop.h"
#include "src/MyServer.h"
#include <sys/resource.h>
int main() {

struct rlimit rl;
getrlimit(RLIMIT_NOFILE, &rl);
printf("rlimit: soft=%ld hard=%ld\n", rl.rlim_cur, rl.rlim_max);

    MyEventLoop *loop = new MyEventLoop();
    MyServer *server = new MyServer(loop,7);
    loop->startLoop();
    return 0;
}
