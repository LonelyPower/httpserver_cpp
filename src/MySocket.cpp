#include "MySocket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

MySocket::MySocket() {
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr_, 0, sizeof(addr_));

    if (sockfd_ < 0) {
        perror("socket create failed");
    }
    // fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
}

MySocket::~MySocket() {
    if (sockfd_ >= 0) {
        close(sockfd_);
    }
}

void MySocket::bindAddr(const std::string& ip, int port)
{
    
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);

    if (bind(sockfd_, (sockaddr *)&addr_, sizeof(addr_)) < 0) {
        perror("bind failed");
    }
}

void MySocket::startListen(int backlog) {
    if (listen(sockfd_, backlog) < 0) {
        perror("listen failed");
    }
}

int MySocket::acceptConn() {
    sockaddr_in c_addr{};
    socklen_t c_addr_len = sizeof(c_addr);
    int cfd = accept4(sockfd_, (sockaddr *)&c_addr, &c_addr_len,
                  SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (cfd < 0) {
        perror("accept failed");
    }
    // fcntl(cfd, F_SETFL, fcntl(sockfd_, F_GETFL) | O_NONBLOCK);
    return cfd;
}

void MySocket:: connectToServer(const std::string& ip, int port)
{
        addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = inet_addr(ip.c_str());
    addr_.sin_port = htons(port);

    if (connect(sockfd_, (sockaddr *)&addr_, sizeof(addr_)) < 0) {
        perror("bind failed");
    }
}