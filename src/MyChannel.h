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

    // void addToEpoll(int socketfd,uint32_t  mode);
    void setEvents(uint32_t ev) { events_ = ev; }
    void setRevents(uint32_t rev) { revents_ = rev; }
    void setInEpoll(bool in) { inEpoll_ = in; }
    void setCallback(const std::function<void()> cb) ;
    void handleEvent();

    int  getFd() const { return fd_; } 
    // std::vector<epoll_event>& getEvents() { return events; } 
        uint32_t getEvents() const { return events_; } 
        uint32_t getRevents() const { return revents_; } 
        bool isInEpoll() const { return inEpoll_; } 

private:
        int fd_;
        uint32_t events_;
        uint32_t revents_;
        bool inEpoll_;
        std::function<void()> readCallback_;
};
