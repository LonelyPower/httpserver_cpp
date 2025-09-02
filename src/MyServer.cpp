#include "MyServer.h"
#include<iostream>
#include <unistd.h>  
MyServer::MyServer(MyEventLoop *loop,const std::string& ip, int port) : event_loop(loop)
{
    // serv_sock = new MySocket();
    // serv_sock->bindAddr(ip, port);
    // serv_sock->startListen();
    acceptor = new MyAcceptor(loop, ip, port);
    acceptor->setCallBack([this](int c_sockfd) {
        this->newConnection(c_sockfd);
    });
    // MyEpoll epoll;
    // serv_channel = new MyChannel(serv_sock->getFd(), EPOLLIN);
    // std::function<void()> cb = std::bind(&MyServer::handleServerEvent, this);
    // serv_channel->setCallBack(cb);
    // event_loop->updateChannel(serv_channel);
}

MyServer::~MyServer()
{
    // delete serv_sock;
    delete serv_channel;
}

void MyServer::handleClientEvent(MyChannel* channel) {
    int c_sockfd = channel->getFd();
    char buf[1024];
    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));

    if (read_bytes > 0) {
        printf("message from client fd %d: %s\n", c_sockfd, buf);
        write(c_sockfd, buf, read_bytes);
    } else if (read_bytes == 0) {
        printf("EOF, client fd %d disconnected\n", c_sockfd);
        event_loop->delChannel(channel);
        close(c_sockfd);
        delete channel;
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return;
        else if (errno == EINTR) return;
        else {
            perror("read error");
            event_loop->delChannel(channel);
            close(c_sockfd);
            delete channel;
        }
    }
}


void MyServer::newConnection(int c_sockfd)
{
    // int c_sockfd = serv_sock->acceptConn();
    MyChannel *clientChannel = new MyChannel(c_sockfd, EPOLLIN);

clientChannel->setCallBack([this, clientChannel]() {
    this->handleClientEvent(clientChannel);
});

    event_loop->updateChannel(clientChannel);
    printf("new client fd %d connected!\n", c_sockfd);
}

// void MyServer::handleServerEvent()
// {
//     newConnection();
// }


