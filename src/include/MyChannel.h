#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
#include <functional>
#include "config.h"
class MyChannel
{
public:
        MyChannel(int fd, int32_t events );
        // MyChannel(int fd,int32_t events=EPOLLIN | EPOLLET);
        ~MyChannel();

        // void addToEpoll(int socketfd,uint32_t  mode);
        void setEvents(uint32_t ev) { channel_events_ = ev; }
        void setReadyEvents(uint32_t rev) { channel_ready_events_ = rev; }
        // void setInEpoll(bool in) { inEpoll_ = in; }
        void setChannelReadCallback(const std::function<void()> cb);
        void setChannelWriteCallback(const std::function<void()> cb);
        void handleEvent();

        int getFd() const { return fd_; }
        // std::vector<epoll_event>& getEvents() { return events; }
        uint32_t getEvents() const { return channel_events_; }
        uint32_t getRevents() const { return channel_ready_events_; }
        // bool isInEpoll() const { return inEpoll_; }

private:
        int fd_;
        uint32_t channel_events_;
        uint32_t channel_ready_events_;
        // bool inEpoll_;

        std::function<void()> channel_read_callback_;
        std::function<void()> channel_write_callback_;
        std::function<void()> channel_close_callback_;
        std::function<void()> channel_error_callback_;
};
