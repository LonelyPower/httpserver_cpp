#include "MySocket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

MySocket::MySocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));

    if (sockfd < 0) {
        perror("socket create failed");
    }
    // fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
}

MySocket::~MySocket() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void MySocket::bindAddr(const std::string& ip, int port)
{
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
    }
}

void MySocket::startListen(int backlog) {
    if (listen(sockfd, backlog) < 0) {
        perror("listen failed");
    }
}

int MySocket::acceptConn() {
    sockaddr_in c_addr{};
    socklen_t c_addr_len = sizeof(c_addr);
    int cfd = accept4(sockfd, (sockaddr *)&c_addr, &c_addr_len,
                  SOCK_NONBLOCK | SOCK_CLOEXEC);

    if (cfd < 0) {
        perror("accept failed");
    }
    // fcntl(cfd, F_SETFL, fcntl(sockfd, F_GETFL) | O_NONBLOCK);
    return cfd;
}

void MySocket:: connectToServer(const std::string& ip, int port)
{
        addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (connect(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
    }
}