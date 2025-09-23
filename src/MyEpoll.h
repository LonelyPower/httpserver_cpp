#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
#include "MyChannel.h"

#define MAX_EVENTS 65500

class MyEpoll
{
public:
    MyEpoll(int maxEvents = MAX_EVENTS);
    ~MyEpoll();

    void updateChannel(MyChannel *channel); // 注册或修改 Channel
    void delChannel(MyChannel *channel);    // 删除 Channel
    // int waitEvents(int maxEvents=MAX_EVENTS, int timeout = -1);          // 等待事件
    std::vector<MyChannel *> getActiveChannels(int timeout = -1); // 返回活跃的 Channel

    int getFd() const { return epfd_; }
    std::vector<epoll_event> &getEvents() { return events_; }

private:
    int epfd_;
    // int MAX_EVENTS;
    std::vector<epoll_event> events_;
    // int activeCount_;   // 上一次 epoll_wait 返回的事件数量
};
