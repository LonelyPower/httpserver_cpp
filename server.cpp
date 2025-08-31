#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>
#include "Mysocket.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main() {
    
    // 使用封装类
    Mysocket server;
    server.bindAddr("127.0.0.1", 8888);
    server.startListen();

    int s_sockfd = server.getFd();

    int epfd = epoll_create1(0);
    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN;
    ev.data.fd = s_sockfd;
    setnonblocking(s_sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, s_sockfd, &ev);

    while (true) {
        int epfdnums = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < epfdnums; i++) {
            if (events[i].data.fd == s_sockfd) {
                // 新连接
                int c_sockfd = server.acceptConn();

                ev.events = EPOLLIN;
                ev.data.fd = c_sockfd;
                setnonblocking(c_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, c_sockfd, &ev);

                printf("new client fd %d connected!\n", c_sockfd);
            } else {
                // 已连接的客户端消息
                while (true) {
                    int c_sockfd = events[i].data.fd;
                    char buf[READ_BUFFER] = {0};
                    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));
                    if (read_bytes > 0) {
                        printf("message from client fd %d: %s\n", c_sockfd, buf);
                        write(c_sockfd, buf, read_bytes);
                    } else if (read_bytes == 0) {
                        printf("EOF, client fd %d disconnected\n", c_sockfd);
                        close(c_sockfd);
                        break;
                    } else if (read_bytes == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        } else if (errno == EINTR) {
                            continue;
                        } else {
                            perror("read error");
                            close(c_sockfd);
                            break;
                        }
                    }
                }
            }
        }
    }

    return 0;
}
