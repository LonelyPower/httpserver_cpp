#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include "MyEpoll.h"
#include "MyEventLoop.h"
#include "MySocket.h"
#include "MyChannel.h"
class MyServer {
public:
    MyServer(MyEventLoop *loop,const std::string& ip="127.0.0.1", int port=8888);                        // 构造函数
    ~MyServer();                         // 析构函数

    void handleClientEvent(MyChannel* channel);
    // void handleClientEvent(int c_sockfd);
    void newConnection();

private:
    MyEventLoop *event_loop;
    MySocket* serv_sock;
    MyChannel* serv_channel;

};