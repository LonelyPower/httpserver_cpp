#include "MyEpoll.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstring>    // perror
#include <cstdlib>
#include <fcntl.h>


MyEpoll::MyEpoll(int maxEvents)
{
    activeCount = 0;
    // MAX_EVENTS = 1024;
    events.resize(maxEvents);
    epfd = epoll_create1(0);
    if (epfd == -1)
    {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
}

MyEpoll::~MyEpoll()
{
    if (epfd != -1)
    {
        close(epfd);
    }
}

void MyEpoll::updateChannel(MyChannel* channel) {
    struct epoll_event ev;
    ev.events = channel->getEvents();
    ev.data.ptr = channel;   // 存储指针，方便回调

    if (!channel->isInEpoll()) {
        if (epoll_ctl(epfd, EPOLL_CTL_ADD, channel->getFd(), &ev) == -1) {
            perror("epoll_ctl: add");
        } else {
            channel->setInEpoll(true);
        }
    } else {
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, channel->getFd(), &ev) == -1) {
            perror("epoll_ctl: mod");
        }
    }
}

void MyEpoll::delChannel(MyChannel* channel) {
    if (channel->isInEpoll()) {
        if (epoll_ctl(epfd, EPOLL_CTL_DEL, channel->getFd(), nullptr) == -1) {
            perror("epoll_ctl del");
        }
        channel->setInEpoll(false);
    }
}



int MyEpoll::waitEvents(int maxEvents, int timeout)
{
    return epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
}

std::vector<MyChannel*> MyEpoll::getActiveChannels(int timeout)
{
    std::vector<MyChannel*> activeChannels;
    int nfds = epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
    // errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        MyChannel *ch = (MyChannel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

// void MyEpoll::delChannel(MyChannel* channel) {
//     int fd = channel->getFd();
//     struct epoll_event ev;
//     memset(&ev, 0, sizeof(ev));
//     epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);
//     // channels.erase(fd);   // 如果你在 map 里保存了 Channel 指针
// }