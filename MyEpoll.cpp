#include "MyEpoll.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstdlib>
#include <fcntl.h>


MyEpoll::MyEpoll()
{
    MAX_EVENTS = 1024;
    events.resize(MAX_EVENTS);
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

void MyEpoll::addToEpoll(int socketfd,uint32_t mode)
{
    struct epoll_event ev;
    ev.events = mode;
    ev.data.fd = socketfd;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &ev) == -1)
    {
        perror("epoll_ctl: add");
    }
    fcntl(socketfd, F_SETFL, fcntl(socketfd, F_GETFL) | O_NONBLOCK);
}

void MyEpoll::addChannel(MyChannel* channel)
{
    struct epoll_event ev;
    ev.events = channel->getEvents();
    ev.data.ptr = channel;

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, channel->getFd(), &ev) == -1)
    {
        perror("epoll_ctl: add");
    }
    fcntl(channel->getFd(), F_SETFL, fcntl(channel->getFd(), F_GETFL) | O_NONBLOCK);
}

int MyEpoll::waitEvents(int MAX_EVENTS, int timeout)
{
    return epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
}

std::vector<MyChannel*> MyEpoll::getActiveChannels(int timeout=-1)
{
    std::vector<MyChannel*> activeChannels;
    int nfds = epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        MyChannel *ch = (MyChannel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}