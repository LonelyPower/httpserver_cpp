#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
#include <functional>

class MyChannel {
public:
    MyChannel(int fd,int32_t events=EPOLLIN | EPOLLET);                          
    ~MyChannel();                        

    void addToEpoll(int socketfd,uint32_t  mode);
    void setEvents(uint32_t ev) { events = ev; }
    void setRevents(uint32_t rev) { revents = rev; }
    void setInEpoll(bool in) { inEpoll = in; }
    void setCallback(const std::function<void()>& cb) ;
    void handleEvent();

    int  getFd() const { return fd; } 
    // std::vector<epoll_event>& getEvents() { return events; } 
    uint32_t getEvents() const { return events; } 
    uint32_t getRevents() const { return revents; } 
    bool isInEpoll() const { return inEpoll; } 

private:
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    std::function<void()> readCallback;
};
