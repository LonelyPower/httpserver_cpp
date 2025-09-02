#include "MyAcceptor.h"

MyAcceptor::MyAcceptor(MyEventLoop *loop,const std::string& ip, int port) : event_loop(loop)
{
    serv_sock = new MySocket();
    serv_sock->bindAddr(ip, port);
    serv_sock->startListen();

    // MyEpoll epoll;
    serv_channel = new MyChannel(serv_sock->getFd(), EPOLLIN);
    std::function<void()> cb = std::bind(&MyAcceptor::handleConnection, this);
    serv_channel->setCallBack(cb);
    event_loop->updateChannel(serv_channel);
}

void MyAcceptor::handleConnection() 
{

    int cfd = serv_sock->acceptConn();
    if (newConnCallBack) {
        newConnCallBack(cfd); // 交给 Server
    }
}

void MyAcceptor::setCallBack(const std::function<void(int)>& cb) 
{
    newConnCallBack = std::move(cb); 
}