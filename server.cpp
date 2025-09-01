#include "src/MyEventLoop.h"
#include "src/MyServer.h"

int main() {
    MyEventLoop *loop = new MyEventLoop();
    MyServer *server = new MyServer(loop);
    loop->startLoop();
    return 0;
}