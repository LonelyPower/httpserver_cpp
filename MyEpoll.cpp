#include "MyEpoll.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstdlib>
// finish this cpp
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
}

int MyEpoll::waitEvents(int MAX_EVENTS, int timeout)
{
    return epoll_wait(epfd, events.data(), MAX_EVENTS, timeout);
}
