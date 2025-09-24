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
    MyConnection(MyEventLoop *loop, int c_sockfd);
    ~MyConnection();

    void setConnectionCallback(std::function<void(int)> cb);


    MyBuffer &getInputBuffer() { return inputBuffer_; }
    MyBuffer &getOutputBuffer() { return outputBuffer_; }

private:
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