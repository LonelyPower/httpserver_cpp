#include "MyEventLoop.h"
#include "MyChannel.h"
#include "MyEpoll.h"
#include <vector>

MyEventLoop::MyEventLoop()
    : epoll(new MyEpoll()), isQuit(false) {}

MyEventLoop::~MyEventLoop()
{
    delete epoll;
}

void MyEventLoop::startLoop()
{
    while (!this->isQuit)
    {
        // epoll->waitEvents();                                                  // 先等待事件
        std::vector<MyChannel *> activeChannels = epoll->getActiveChannels(); // 获取就绪的 Channel
        for (auto *ch : activeChannels)
        {
            ch->handleEvent(); // 分发事件
        }
    }
}

void MyEventLoop::updateChannel(MyChannel *channel)
{
    epoll->updateChannel(channel); // 或者叫 updateChannel
}
    
void MyEventLoop::delChannel(MyChannel *channel)
{
    epoll->delChannel(channel);
}
