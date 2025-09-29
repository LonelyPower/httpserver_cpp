#include "MyChannel.h"
#include <unistd.h> // close
#include <cstdio>   // perror
#include <cstdlib>
#include <fcntl.h>

MyChannel::MyChannel(int fd, int32_t events)
{
    this->fd_ = fd;
    this->channel_events_ = events;
    this->channel_ready_events_ = 0;
    this->inEpoll_ = false;
}

MyChannel::~MyChannel() {}

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
    else if (channel_ready_events_ & EPOLLOUT)
    {
        if (channel_write_callback_)
        {
            channel_write_callback_();
        }
        // 可以扩展其他事件的处理
    }
    else if (channel_ready_events_ & (EPOLLHUP | EPOLLRDHUP))
    {
        if (channel_close_callback_)
        {
            channel_close_callback_();
        }
    }
    else if (channel_ready_events_ & EPOLLERR)
    {
        if (channel_error_callback_)
        {
            channel_error_callback_();
        }
    }
}


void MyChannel::setChannelReadCallback(const std::function<void()> cb)
{
    // cout << "2 Setting read callback for channel: " << fd_ << endl;
    channel_read_callback_ = std::move(cb);
    // cout << "2 Finished setting read callback for channel: " << fd_ << endl;
}

void MyChannel::setChannelWriteCallback(const std::function<void()> cb)
{
    // cout << "2 Setting write callback for channel: " << fd_ << endl;
    channel_write_callback_ = std::move(cb);
    // cout << "2 Finished setting write callback for channel: " << fd_ << endl;
}

void MyChannel::setChannelCloseCallback(const std::function<void()> cb)
{
    channel_close_callback_ = std::move(cb);
}
void MyChannel::setChannelErrorCallback(const std::function<void()> cb)
{
    channel_error_callback_ = std::move(cb);
}