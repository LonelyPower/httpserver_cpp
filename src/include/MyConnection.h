#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
// #include "MyConnection.h"
#include "MyChannel.h"
#include "MyEventLoop.h"
#include "MyChannel.h"
#include "MyBuffer.h"

class MyConnection
{
public:
    MyConnection(MyEventLoop *loop, int sockfd);
    ~MyConnection();

    void setConnectionCallback(std::function<void(int)> cb);

    // 对于缓冲区的封装
    std::string readFromConnection(){ return inputBuffer_.getContentAsString(); }
    std::string readFromConnection(size_t len){ return inputBuffer_.getContentAsString(len); }
    void addToBuffer(const char *data, size_t len) { outputBuffer_.addToBuffer(data, len); }
    void addToBuffer(const std::string &data) { outputBuffer_.addToBuffer(data); }
    ssize_t writeToSocket() { return outputBuffer_.writeFromBuffer(socket_fd_); }
    size_t getUnreadSize() const { return inputBuffer_.getUnreadSize(); }

    MyBuffer &getInputBuffer() { return inputBuffer_; }
    MyBuffer &getOutputBuffer() { return outputBuffer_; }

private:
    int socket_fd_;
    MyEventLoop *event_loop_;
    std::function<void(int)> connection_callback_;
    MyChannel *channel_{nullptr};
    MyBuffer inputBuffer_;
    MyBuffer outputBuffer_;
};

// void handleClientEvent(MyChannel* channel);
// void setMessageCallback(const std::function<void(MyConnection*, int)>& cb,int fd);
// void handleConnection();
// void handleServerEvent();
// void handleClientEvent(int c_sockfd);
// void newConnection();