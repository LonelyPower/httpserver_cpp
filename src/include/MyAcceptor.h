#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include "MyEpoll.h"
#include "MyEventLoop.h"
#include "MySocket.h"
#include "MyChannel.h"
#include "config.h"
using AcceptorCallback = std::function<void(MySocket)>;
class MyAcceptor
{
public:
    MyAcceptor(MyEventLoop *loop, const std::string &ip, int port); // 构造函数
    ~MyAcceptor();                                                  // 析构函数

    // void handleClientEvent(MyChannel* channel);
    void setAcceptorCallback(AcceptorCallback cb);
    void handleConnection();
    // void handleServerEvent();
    // void handleClientEvent(int c_sockfd);
    // void newConnection();

private:
    MyEventLoop *event_loop_;
    MySocket server_sock_;
    MyChannel *server_channel_;

    std::function<void(int)> acceptor_callback_;
};
