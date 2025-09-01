#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "MySocket.h"
#include "MyChannel.h"
#define BUFFER_SIZE 1024 

int main() 
{

    // 使用封装类
    MySocket client;
    client.connectToServer("127.0.0.1", 8888);
    int sockfd = client.getFd();
    
    while (true)
    {
        char buf[BUFFER_SIZE]; // 在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf));
        if (write_bytes == -1)
        {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if (read_bytes > 0)
        {
            printf("message from server: %s\n", buf);
        }
        else if (read_bytes == 0)
        {
            printf("server socket disconnected!\n");
            break;
        }
        else if (read_bytes == -1)
        {
            close(sockfd);
        }
    }

    close(sockfd);
    return 0;
}
