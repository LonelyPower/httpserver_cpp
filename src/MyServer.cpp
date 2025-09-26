#include "MyServer.h"
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <cstdio>     // 添加这行，用于 printf 和 snprintf
#include <cstring>  
MyServer::MyServer(MyEventLoop *loop, int poolsize, const std::string &ip, int port) : event_loop_(loop)
{
    thread_pool_ = new MyThreadPool(poolsize);
    thread_pool_->start();

    acceptor_ = new MyAcceptor(loop, ip, port);
    acceptor_->setAcceptorCallBack([this](int c_sockfd)
                           { this->handleServerEvent(c_sockfd); });
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
    // cout << "6 Handling client event for fd: " << c_sockfd << endl;
    auto it = connections_.find(c_sockfd);
    if (it == connections_.end())
    {
          return;
    }
      
    MyConnection *conn = it->second;
    MyBuffer &in = conn->getInputBuffer();
    MyBuffer &out = conn->getOutputBuffer();

    // 从输入缓冲区拿到本次读到的数据（在 MyConnection 的 Channel 回调中已读入）
    if (conn->getUnreadSize() > 0)
    {
        std::string msg = conn->readFromConnection();
        printf("message from client fd %d: %s\n", c_sockfd, msg.c_str());

        //  返回最小 HTTP 响应，便于 wrk 正常统计
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

        conn->addToBuffer(header, hdrLen);
        conn->addToBuffer(kBody, bodyLen);
        if (conn->writeToSocket() < 0)
        {
            if (errno != EAGAIN)
            {
                // 其他错误，关闭连接
                printf("write error on fd %d, closing connection\n", c_sockfd);
                delete conn;
                connections_.erase(it);
                return;
            }
            // EAGAIN 情况下，输出缓冲区数据未写完，等待下一次可写事件
        }
    }
    // cout << "6 Finished handling client event for fd: " << c_sockfd << endl;
}

void MyServer::newConnection(int c_sockfd)
{
    MyEventLoop *ioLoop = thread_pool_->getNextLoop();

    MyConnection *conn = new MyConnection(ioLoop, c_sockfd);
    conn->setConnectionCallback(
        [this](int fd)
        { 
            // cout<<"5 Message callback invoked for connection fd: " << fd << endl;
            this->handleClientEvent(fd);
            // cout<<"5 Finished message callback for connection fd: " << fd << endl;
        });

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
