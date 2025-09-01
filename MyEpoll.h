#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
#include "MyChannel.h"
class MyEpoll {
public:
    MyEpoll();                          
    ~MyEpoll();                        

    void addToEpoll(int socketfd,uint32_t  mode);
    void addChannel(MyChannel* channel);
    int waitEvents(int MAX_EVENTS=1024,int timeout=-1);

    int  getFd() const { return epfd; } 
    std::vector<epoll_event>& getEvents() { return events; } 
    std::vector<MyChannel*> getActiveChannels(int timeout=-1);
    void delChannel(MyChannel* channel);

private:
    int MAX_EVENTS;
    int epfd;
    std::vector<epoll_event> events;
};
