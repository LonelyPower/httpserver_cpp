#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "MySocket.h"
#include "MyEpoll.h"
#include "MyChannel.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

int main()
{

    // 使用封装类
    MySocket server;
    server.bindAddr("127.0.0.1", 8888);
    server.startListen();

    MyEpoll epoll;
    MyChannel channel(server.getFd(), EPOLLIN);
    epoll.addChannel(&channel);
    // fcntl(server.getFd(), F_SETFL, fcntl(server.getFd(), F_GETFL) | O_NONBLOCK);

    while (true)
    {
        std::vector<MyChannel*> activeChannels = epoll.getActiveChannels();
        // int readyEvents = epoll.waitEvents();

        for (auto activeChannel : activeChannels)
        {
            if (activeChannel->getFd() == server.getFd())
            {
                // 新连接
                int c_sockfd = server.acceptConn();
                MyChannel* clientChannel = new MyChannel(c_sockfd, EPOLLIN);
                epoll.addChannel(clientChannel);
                // fcntl(c_sockfd, F_SETFL, fcntl(c_sockfd, F_GETFL) | O_NONBLOCK);
                printf("new client fd %d connected!\n", c_sockfd);
            }
            else
            {
                // 已连接的客户端消息
                while (true)
                {
                    int c_sockfd = activeChannel->getFd();
                    char buf[READ_BUFFER] = {0};
                    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));
                    if (read_bytes > 0)
                    {
                        printf("message from client fd %d: %s\n", c_sockfd, buf);
                        write(c_sockfd, buf, read_bytes);
                    }
                    else if (read_bytes == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", c_sockfd);
                        close(c_sockfd);
                        break;
                    }
                    else if (read_bytes == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            break;
                        }
                        else if (errno == EINTR)
                        {
                            continue;
                        }
                        else
                        {
                            perror("read error");
                            close(c_sockfd);
                            break;
                        }
                    }
                }
            }
        }

        
    }return 0;
}