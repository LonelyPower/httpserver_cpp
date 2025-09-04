#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include "MyEpoll.h"
#include "MyEventLoop.h"
#include "MySocket.h"
#include "MyChannel.h"
class MyAcceptor {
public:
    MyAcceptor(MyEventLoop *loop,const std::string& ip="127.0.0.1", int port=8888);                        // 构造函数
    ~MyAcceptor();                         // 析构函数

    // void handleClientEvent(MyChannel* channel);
    void setCallBack(const std::function<void(int)> cb);
    void handleConnection();
    // void handleServerEvent();
    // void handleClientEvent(int c_sockfd);
    void newConnection();

private:
    MyEventLoop *event_loop_;
    MySocket* serv_sock_;
    MyChannel* serv_channel_;
    std::function<void(int)> acceptor_callback_;




};