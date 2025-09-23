#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include "MyEpoll.h"
class MyEventLoop
{
public:
    MyEventLoop();  // 构造函数
    ~MyEventLoop(); // 析构函数

    void updateChannel(MyChannel *channel); // 添加通道
    void delChannel(MyChannel *channel);    // 删除通道
    void startLoop();                       // 事件循环
    void quitLoop() { isQuit_ = true; }

private:
    MyEpoll *epoll_;
    bool isQuit_;
    // int epoll_fd;
    // std::vector<MyChannel*> channels;
};