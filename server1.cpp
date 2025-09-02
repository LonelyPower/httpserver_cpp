#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#include "src/MySocket.h"
#include "src/MyEpoll.h"
#include "src/MyChannel.h"
#include "src/MyEventLoop.h"
#include "src/MyServer.h"
#include <iostream>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void handleRead(MyEpoll &epoll, MyChannel *activeChannel);

int main()
{

    // 使用封装类
    MySocket server;
    server.bindAddr("127.0.0.1", 8888);
    server.startListen();

    MyEpoll epoll;
    MyChannel channel(server.getFd(), EPOLLIN);
    epoll.updateChannel(&channel);
    // fcntl(server.getFd(), F_SETFL, fcntl(server.getFd(), F_GETFL) | O_NONBLOCK);

    while (true)
    {
        std::vector<MyChannel *> activeChannels = epoll.getActiveChannels();
        // int readyEvents = epoll.waitEvents();
        for (auto activeChannel : activeChannels)
        {
            if (activeChannel->getFd() == server.getFd())
            {
                // 新连接
                int c_sockfd = server.acceptConn();
                MyChannel *clientChannel = new MyChannel(c_sockfd, EPOLLIN);

                // 绑定时捕获 epoll 和 clientChannel
                clientChannel->setCallback(
                    std::bind(handleRead, std::ref(epoll), clientChannel));

                epoll.updateChannel(clientChannel);
                printf("new client fd %d connected!\n", c_sockfd);
            }
            else
            {
                // 已连接的客户端消息
                activeChannel->handleEvent();
            }
        }
    }
    return 0;
}

void handleRead(MyEpoll &epoll, MyChannel *activeChannel)
{
    int c_sockfd = activeChannel->getFd();
    char buf[READ_BUFFER];
    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));

    if (read_bytes > 0)
    {
        printf("message from client fd %d: %s\n", c_sockfd, buf);
        ssize_t n = write(c_sockfd, buf, read_bytes);
        if (n == -1)
            perror("write error");
    }
    else if (read_bytes == 0)
    {
        // 客户端关闭
        printf("EOF, client fd %d disconnected\n", c_sockfd);

        epoll.delChannel(activeChannel); // 从 epoll 移除
        close(c_sockfd);
        delete activeChannel; // 释放 Channel
    }
    else if (read_bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return; // 数据读完了
        }
        else if (errno == EINTR)
        {
            handleRead(epoll, activeChannel); // 被信号中断，重试
        }
        else
        {
            perror("read error");
            epoll.delChannel(activeChannel);
            close(c_sockfd);
            delete activeChannel;
        }
    }
}
