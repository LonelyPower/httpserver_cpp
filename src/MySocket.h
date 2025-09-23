#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in

class MySocket
{
public:
    MySocket();  // 构造函数：创建 socket
    ~MySocket(); // 析构函数：关闭 socket

    void bindAddr(const std::string &ip, int port); // 绑定 IP 和端口
    void startListen(int backlog = 5);              // 开始监听
    int acceptConn();                               // 接受连接，返回客户端 fd
    void connectToServer(const std::string &ip, int port);

    int getFd() const { return sockfd_; } // 获取 socket fd

private:
    int sockfd_;
    sockaddr_in addr_;
};
