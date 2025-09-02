#include "MyChannel.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstdlib>
#include <fcntl.h>


MyChannel::MyChannel(int fd,int32_t events)
{
    this->fd = fd;
    this->events = events;
    this->revents = 0;
    this->inEpoll = false;
}

MyChannel::~MyChannel(){}

void MyChannel::handleEvent() 
{
    if (revents & EPOLLIN) 
    {
        if (readCallback)
        {
            readCallback();
        } 
    }
    // 可以扩展其他事件的处理
}
void MyChannel::setCallback(const std::function<void()>& cb) 
{
    readCallback = std::move(cb); 
}
// void MyChannel::setEvents(uint32_t ev) {
//     this->events = ev;
// }

// void MyChannel::setRevents(uint32_t rev) {
//     this->revents = rev;
// }
