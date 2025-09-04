#include "MyEventLoop.h"
#include "MyChannel.h"
#include "MyEpoll.h"
#include <vector>
#include <errno.h>
#include "MyConnection.h"

MyConnection::MyConnection(MyEventLoop* loop, int c_sockfd) : event_loop_(loop), message_callback_(nullptr)
{
    channel_ = new MyChannel(c_sockfd, EPOLLIN);

    channel_->setCallback([this]() {
        // Loop read for EPOLLET until EAGAIN
        for (;;) {
            int savedErrno = 0;
            ssize_t n = this->inputBuffer_.readFd(channel_->getFd(), &savedErrno);
            if (n > 0) {
                continue; // try read more until EAGAIN
            } else if (n == 0) {
                // peer closed; let upper layer handle close if desired
                break;
            } else { // n < 0
                if (savedErrno == EAGAIN || savedErrno == EWOULDBLOCK) {
                    break; // no more data
                }
                if (savedErrno == EINTR) {
                    continue; // retry
                }
                // other errors: break and let upper layer decide
                break;
            }
        }
        if (this->message_callback_) this->message_callback_(channel_->getFd());
    });

    event_loop_->updateChannel(channel_);
    printf("new client fd %d connected!\n", c_sockfd);
}
MyConnection::~MyConnection()
{
    if (channel_) {
        event_loop_->delChannel(channel_);
        delete channel_;
        channel_ = nullptr;
    }
}

void MyConnection::setMessageCallback(std::function<void( int)> cb) 
{
    message_callback_ = std::move(cb);
}