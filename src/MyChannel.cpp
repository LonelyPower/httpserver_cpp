#include "MyChannel.h"
#include <unistd.h>   // close
#include <cstdio>    // perror
#include <cstdlib>
#include <fcntl.h>


MyChannel::MyChannel(int fd,int32_t events)
{
    this->fd_ = fd;
    this->channel_events_ = events;
    this->channel_ready_events_ = 0;
    this->inEpoll_ = false;
}

MyChannel::~MyChannel(){}

void MyChannel::handleEvent() 
{
    if (channel_ready_events_ & EPOLLIN) 
    {
        if (channel_callback_)
        {
            // cout << "2 Invoking read callback for channel: " << fd_ << endl;
            // printf("2 Invoking read callback for channel: %d\n", fd_);
            channel_callback_();
            // printf("2 Finished read callback for channel: %d\n", fd_);
            // cout << "2 Finished read callback for channel: " << fd_ << endl;
        } 
    }
    // 可以扩展其他事件的处理
}
void MyChannel::setChannelCallback(const std::function<void()> cb) 
{
    // cout << "2 Setting read callback for channel: " << fd_ << endl;
    channel_callback_ = std::move(cb); 
    // cout << "2 Finished setting read callback for channel: " << fd_ << endl;
}
