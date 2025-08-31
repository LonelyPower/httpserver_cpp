#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd)
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int main()
{
    int s_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // 初始化
    //  struct sockaddr_in s_addr;
    sockaddr_in s_addr{};

    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    s_addr.sin_port = htons(8888);

    bind(s_sockfd, (sockaddr *)&s_addr, sizeof(s_addr));
    // 开始监听
    listen(s_sockfd, SOMAXCONN);

    int epfd = epoll_create1(0);
    struct epoll_event events[MAX_EVENTS], ev;
    ev.events = EPOLLIN;   // 在代码中使用了ET模式，且未处理错误，在day12进行了修复，实际上接受连接最好不要用ET模式
    ev.data.fd = s_sockfd; // 该IO口为服务器socket fd
    setnonblocking(s_sockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, s_sockfd, &ev);

    // 用来保存 哪个客户端连接了我。
    //  struct sockaddr_in c_addr;

    // bzero(&c_addr, sizeof(c_addr));

    

    while (true)
    {
        int epfdnums = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < epfdnums; i++)
        {
            if (events[i].data.fd == s_sockfd)
            {
                sockaddr_in c_addr{};
                socklen_t c_addr_len = sizeof(c_addr);
                int c_sockfd = accept(s_sockfd, (sockaddr *)&c_addr, &c_addr_len);


                ev.events = EPOLLIN;
                ev.data.fd = c_sockfd; // 该IO口为服务器socket fd
                setnonblocking(c_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, c_sockfd, &ev);

                printf("new client fd %d! IP: %s Port: %d\n", c_sockfd, inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
            }
            else
            {
                while (true)
                {
                    int c_sockfd=events[i].data.fd;
                    char buf[1024] = {0};
                    ssize_t read_bytes = read(c_sockfd, buf, sizeof(buf));
                    if (read_bytes > 0)
                    {
                        printf("message from client fd %d: %s\n", c_sockfd, buf);
                        write(c_sockfd, buf, read_bytes); // 只回写实际长度
                    }
                    else if (read_bytes == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd); // 关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                    else if (read_bytes == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            // 没有数据，非阻塞返回，稍后再试
                            break;
                        }
                        else if(errno == EINTR)
                        { // 客户端正常中断、继续读取
                            printf("continue reading");
                            continue;
                        }
                    }
                }
            }
        }
    }

    close(s_sockfd);
    return 0;
}
