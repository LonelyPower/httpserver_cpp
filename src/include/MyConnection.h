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
#include "MySocket.h"

class MyConnection
{
public:
    MyConnection(MyEventLoop *loop, MySocket&& client_socket);
    ~MyConnection();

    void setConnectionCallback(std::function<void(int)> cb);

    // 对于缓冲区的封装
    std::string readFromConnection(){ return inputBuffer_.getContentAsString(); }
    std::string readFromConnection(size_t len){ return inputBuffer_.getContentAsString(len); }
    void addToBuffer(const char *data, size_t len) { outputBuffer_.addToBuffer(data, len); }
    void addToBuffer(const std::string &data) { outputBuffer_.addToBuffer(data); }
    ssize_t writeToSocket() { return outputBuffer_.writeFromBuffer(client_socket_.getFd()); }
    size_t getUnreadSize() const { return inputBuffer_.getUnreadSize(); }

    MyBuffer &getInputBuffer() { return inputBuffer_; }
    MyBuffer &getOutputBuffer() { return outputBuffer_; }

private:
    MySocket client_socket_;
    MyEventLoop *event_loop_;
    std::function<void(int)> connection_callback_;
    MyChannel *channel_;
    MyBuffer inputBuffer_;
    MyBuffer outputBuffer_;
};

