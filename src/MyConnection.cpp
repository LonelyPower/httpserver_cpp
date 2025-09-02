#include "MyConnection.h"

MyConnection::MyConnection(MyEventLoop* loop, int c_sockfd) : event_loop_(loop), message_callback_(nullptr)
{
    MyChannel *clientChannel = new MyChannel(c_sockfd, EPOLLIN);

    clientChannel->setCallback([this, clientChannel]() {
        message_callback_(clientChannel->getFd());
    });

    event_loop_->updateChannel(clientChannel);
    printf("new client fd %d connected!\n", c_sockfd);
}
MyConnection::~MyConnection()
{
    // delete serv_sock_;
    // delete serv_channel_;
}

void MyConnection::setMessageCallback(const std::function<void( int)>& cb,int fd) 
{
    message_callback_ = std::move(cb);
}