#include "src/MyThreadPool.h"
#include "src/MyEventLoop.h"
#include "src/MyChannel.h"
#include <iostream>
#include <unistd.h>   // pipe, write
#include <functional>
#include <cstring>
// 模拟一个事件：往管道里写数据，子线程的 EventLoop 会读到
void simulateEvent(MyEventLoop* loop, int index) {
    int fds[2];
    pipe(fds); // fds[0] 读端, fds[1] 写端

    // 在子线程 loop 上注册一个 Channel，监听读事件
    MyChannel* channel = new MyChannel(fds[0], EPOLLIN);
    channel->setCallback([fds, index]() {
        char buf[64];
        int n = read(fds[0], buf, sizeof(buf));
        if (n > 0) {
            std::cout << "[Sub reactor thread " 
                      << std::this_thread::get_id()
                      << "] got message: " << buf 
                      << " (from task " << index << ")"
                      << std::endl;
        }
    });

    loop->updateChannel(channel);

    // 主线程写数据，触发子线程 loop 的事件
    std::string msg = "hello " + std::to_string(index);
    write(fds[1], msg.c_str(), msg.size());
}

int main() {
    // 启动线程池，假设 3 个 sub reactor
    MyThreadPool pool(3);
    pool.start();

    sleep(1); // 等待线程池启动完成

    // 主线程模拟分配 10 个任务
    for (int i = 0; i < 10; i++) {
        MyEventLoop* loop = pool.getNextLoop(); // 轮询获取 sub reactor
        simulateEvent(loop, i);
        usleep(100000); // 每次间隔 0.1s，方便观察输出
    }

    // 等待子线程处理完
    sleep(2);

    return 0;
}