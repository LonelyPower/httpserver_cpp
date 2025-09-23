#include "MyServer.h"
#include<iostream>
#include <unistd.h>  
#include <errno.h>
MyServer::MyServer(MyEventLoop *loop,int poolsize,const std::string& ip, int port) : event_loop_(loop)
{
    thread_pool_ = new MyThreadPool(poolsize);
    thread_pool_->start();

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

void MyServer::handleClientEvent(int c_sockfd)
{
    auto it = connections_.find(c_sockfd);
    if (it == connections_.end()) return;
    MyConnection* conn = it->second;
    Buffer& in = conn->getInputBuffer();

    // 从输入缓冲区拿到本次读到的数据（在 MyConnection 的 Channel 回调中已读入）
    if (in.readableBytes() > 0)
    {
        std::string msg = in.retrieveAllAsString();
        //printf("message from client fd %d: %s\n", c_sockfd, msg.c_str());
        // 返回最小 HTTP 响应，便于 wrk 正常统计
        static const char kBody[] = "Hello, World\n";
        char header[256];
        int bodyLen = sizeof(kBody) - 1;
        int hdrLen = snprintf(header, sizeof(header),
                              "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/plain; charset=utf-8\r\n"
                              "Content-Length: %d\r\n"
                              "Connection: keep-alive\r\n"
                              "\r\n",
                              bodyLen);
        // 尽量一次写完 header + body（非阻塞写不保证全部写完，但响应很小一般可一次写完）
        ssize_t n1 = write(c_sockfd, header, hdrLen);
        if (n1 < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
            perror("write header error");
            close(c_sockfd);
            connections_.erase(c_sockfd);
            return;
        }
        ssize_t n2 = write(c_sockfd, kBody, bodyLen);
        if (n2 < 0 && !(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)) {
            perror("write body error");
            close(c_sockfd);
            connections_.erase(c_sockfd);
            return;
        }
    }
}


void MyServer::newConnection(int c_sockfd)
{
    MyEventLoop* ioLoop = thread_pool_->getNextLoop();

    MyConnection* conn = new MyConnection(ioLoop, c_sockfd);
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


