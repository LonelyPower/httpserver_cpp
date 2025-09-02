#include "MyServer.h"
#include<iostream>
#include <unistd.h>  
MyServer::MyServer(MyEventLoop *loop,const std::string& ip, int port) : event_loop_(loop)
{

    acceptor_ = new MyAcceptor(loop, ip, port);
    acceptor_->setCallBack([this](int c_sockfd) {
        this->handleServerEvent(c_sockfd);
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
    delete acceptor_;
}

// void MyServer::handleClientEvent(MyChannel* channel) {
void MyServer::handleClientEvent(int c_sockfd) {
    // int c_sockfd = channel->getFd();
    char buf[1024];
    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));

    if (read_bytes > 0) {
        printf("message from client fd %d: %s\n", c_sockfd, buf);
        write(c_sockfd, buf, read_bytes);
    } else if (read_bytes == 0) {
        printf("EOF, client fd %d disconnected\n", c_sockfd);
    // event_loop_->delChannel(channel);
        close(c_sockfd);
        connections_.erase(c_sockfd);

        // delete channel;
    // } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) return;
        else if (errno == EINTR) return;
        else {
            perror("read error");
            // event_loop_->delChannel(channel);
            close(c_sockfd);
            // delete channel;
        }
    }
}


void MyServer::newConnection(int c_sockfd)
{
    MyConnection* conn = new MyConnection(event_loop_, c_sockfd);

    conn->setMessageCallback(
        [this](int fd) { this->handleClientEvent(fd); }
    );

    connections_[c_sockfd] = conn;
    // printf("new client fd %d connected!\n", c_sockfd);
}

// void MyServer::handleServerEvent(int c_sockfd)
// {
//     newConnection(c_sockfd);
// });

//     event_loop_->updateChannel(clientChannel);
//     printf("new client fd %d connected!\n", c_sockfd);

// }

void MyServer::handleServerEvent(int c_sockfd)
{
    newConnection(c_sockfd);
}


