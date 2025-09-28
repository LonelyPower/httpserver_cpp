#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <map>

// #include "MyEpoll.h"
#include "MyEventLoop.h"
#include "MySocket.h"
// #include "MyChannel.h"
#include "MyAcceptor.h"
#include "MyConnection.h"
#include "MyThreadPool.h"
#include "config.h"

class MyServer {
public:
    MyServer(MyEventLoop *loop,int poolsize=POOL_SIZE,const std::string& ip=SERVER_IP, int port=SERVER_PORT);                        // 构造函数
    ~MyServer();                         // 析构函数

    void handleClientEvent(int c_sockfd);
    // void handleClientEvent(MyChannel* channel);
    void handleServerEvent(MySocket client_socket);
    // void handleClientEvent(int c_sockfd);
    void newConnection(MySocket client_socket);

private:
    MyEventLoop *event_loop_;
    MyAcceptor *acceptor_;
    // MySocket* serv_sock;
    // MyChannel* serv_channel_;
    std::map<int, MyConnection*> connections_;
    MyConnection* connections1_{nullptr}; // 用于分析关系测试
    // std::vector<MyThreadPool*> thread_pool_;
    MyThreadPool* thread_pool_; 

};