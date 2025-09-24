#include "MyEventLoop.h"
#include "MyChannel.h"
// #include "MyEpoll.h"
// #include <vector>
#include <errno.h>
#include "MyConnection.h"
#include "config.h"
MyConnection::MyConnection(MyEventLoop *loop, int c_sockfd) : event_loop_(loop), message_callback_(nullptr)
{
    channel_ = new MyChannel(c_sockfd, HANDLE_MODE);

    channel_->setCallback([this]()
                          {
            cout<<"3 Read callback invoked for connection fd: " << channel_->getFd() << endl;
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
        if (this->message_callback_) 
        {
            cout << "4 Invoking message callback for connection fd: " << channel_->getFd() << endl;
            this->message_callback_(channel_->getFd());
            cout << "4 Finished message callback for connection fd: " << channel_->getFd() << endl;
        } });

    event_loop_->updateChannel(channel_);
    // printf("new client fd %d connected!\n", c_sockfd);
}
MyConnection::~MyConnection()
{
    if (channel_)
    {
        event_loop_->delChannel(channel_);
        delete channel_;
        channel_ = nullptr;
    }
}

void MyConnection::setMessageCallback(std::function<void(int)> cb)
{
    message_callback_ = std::move(cb);
}
