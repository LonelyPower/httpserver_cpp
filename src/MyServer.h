#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <map>

#include "MyEpoll.h"
#include "MyEventLoop.h"
#include "MySocket.h"
#include "MyChannel.h"
#include "MyAcceptor.h"
#include "MyConnection.h"

class MyServer {
public:
    MyServer(MyEventLoop *loop,const std::string& ip="127.0.0.1", int port=8888);                        // 构造函数
    ~MyServer();                         // 析构函数

    void handleClientEvent(int c_sockfd);
    // void handleClientEvent(MyChannel* channel);
    void handleServerEvent(int c_sockfd);
    // void handleClientEvent(int c_sockfd);
    void newConnection(int c_sockfd);

private:
    MyEventLoop *event_loop_;
    MyAcceptor *acceptor_;
    // MySocket* serv_sock;
    MyChannel* serv_channel_;
    std::map<int, MyConnection*> connections_;

};