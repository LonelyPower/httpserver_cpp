#include "MyEventLoop.h"

#include "MyEpoll.h"
#include <vector>

MyEventLoop::MyEventLoop()
    : epoll_(new MyEpoll()), isQuit_(false) {}

MyEventLoop::~MyEventLoop()
{
    delete epoll_;
}

void MyEventLoop::startLoop()
{
    while (!this->isQuit_)
    {
        // epoll_->waitEvents();                                                  // 先等待事件
        std::vector<MyChannel *> activeChannels = epoll_->getActiveChannels(); // 获取就绪的 Channel
        for (auto *ch : activeChannels)
        {
            // cout << "1 Handling event for channel: " << ch->fd() << endl;
            ch->handleEvent(); // 分发事件
            // cout << "1 Finished handling event for channel: " << ch->fd() << endl;
        }
    }
}

void MyEventLoop::updateChannel(MyChannel *channel)
{
    epoll_->updateChannel(channel); // 或者叫 updateChannel
}

void MyEventLoop::delChannel(MyChannel *channel)
{
    epoll_->delChannel(channel);
}
