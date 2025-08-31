#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>

class MyEpoll {
public:
    MyEpoll();                          
    ~MyEpoll();                        

    void addToEpoll(int socketfd,uint32_t  mode);
    int waitEvents(int MAX_EVENTS=1024,int timeout=-1);

    int  getFd() const { return epfd; } 
    std::vector<epoll_event>& getEvents() { return events; } 
    
    

private:
    int MAX_EVENTS;
    int epfd;
    std::vector<epoll_event> events;
};
