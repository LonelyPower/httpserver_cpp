#include "MyChannel.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstdlib>
#include <fcntl.h>


MyChannel::MyChannel(int fd,int32_t events)
{
    this->fd_ = fd;
    this->events_ = events;
    this->revents_ = 0;
    this->inEpoll_ = false;
}

MyChannel::~MyChannel(){}

void MyChannel::handleEvent() 
{
    if (revents_ & EPOLLIN) 
    {
        if (readCallback_)
        {
            readCallback_();
        } 
    }
    // 可以扩展其他事件的处理
}
void MyChannel::setCallback(const std::function<void()>& cb) 
{
    readCallback_ = std::move(cb); 
}