#include "MyAcceptor.h"

MyAcceptor::MyAcceptor(MyEventLoop *loop,const std::string& ip, int port) : event_loop_(loop)
{
    serv_sock_ = new MySocket();
    serv_sock_->bindAddr(ip, port);
    serv_sock_->startListen();

    // MyEpoll epoll;
    serv_channel_ = new MyChannel(serv_sock_->getFd(), EPOLLIN);
    std::function<void()> cb = std::bind(&MyAcceptor::handleConnection, this);
    serv_channel_->setCallback(cb);
    event_loop_->updateChannel(serv_channel_);
}
MyAcceptor::~MyAcceptor()
{
    delete serv_channel_;
    delete serv_sock_;
}
void MyAcceptor::handleConnection() 
{

    int cfd = serv_sock_->acceptConn();
    if (acceptor_callback_) {
        acceptor_callback_(cfd); // 交给 Server
    }
}

void MyAcceptor::setCallBack(const std::function<void(int)>& cb) 
{
    acceptor_callback_ = std::move(cb); 
}