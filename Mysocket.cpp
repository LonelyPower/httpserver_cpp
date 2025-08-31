#include "Mysocket.h"
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Mysocket::Mysocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));

    if (sockfd < 0) {
        perror("socket create failed");
    }
}

Mysocket::~Mysocket() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void Mysocket::bindAddr(const std::string& ip, int port)
{
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
    }
}

void Mysocket::startListen(int backlog) {
    if (listen(sockfd, backlog) < 0) {
        perror("listen failed");
    }
}

int Mysocket::acceptConn() {
    sockaddr_in c_addr{};
    socklen_t c_addr_len = sizeof(c_addr);
    int cfd = accept(sockfd, (sockaddr *)&c_addr, &c_addr_len);
    if (cfd < 0) {
        perror("accept failed");
    }
    return cfd;
}

void Mysocket:: connecttoServer(const std::string& ip, int port)
{
        addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);

    if (connect(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
    }
}