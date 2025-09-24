#include "MyEventLoop.h"
#include "MyChannel.h"
// #include "MyEpoll.h"
// #include <vector>
#include <errno.h>
#include "MyConnection.h"
#include "config.h"
MyConnection::MyConnection(MyEventLoop *loop, int c_sockfd) : event_loop_(loop), inputBuffer_(), outputBuffer_(), connection_callback_(nullptr)
{
    channel_ = new MyChannel(c_sockfd, HANDLE_MODE);

    channel_->setCallback([this]()
                          {
            // cout<<"3 Read callback invoked for connection fd: " << channel_->getFd() << endl;
        // Loop read for EPOLLET until EAGAIN
        this->inputBuffer_.readToBuffer(channel_->getFd());
        // cout<<"3 Finished reading data for connection fd: " << channel_->getFd() << endl;
        if (this->connection_callback_)
        {
            // cout << "4 Invoking connection callback for connection fd: " << channel_->getFd() << endl;
            this->connection_callback_(channel_->getFd());
            // cout << "4 Finished connection callback for connection fd: " << channel_->getFd() << endl;
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

void MyConnection::setConnectionCallback(std::function<void(int)> cb)
{
    connection_callback_ = std::move(cb);
}
