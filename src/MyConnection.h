#pragma once
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/epoll.h>
#include <vector>
// #include "MyConnection.h"
#include "MyChannel.h"
#include "MyEventLoop.h"
#include "MyChannel.h"


#define MAX_EVENTS 1024

class MyConnection {
public:
    MyConnection(MyEventLoop *loop,int c_sockfd);                       
    ~MyConnection();

    // void handleClientEvent(MyChannel* channel);
    void setMessageCallback(const std::function<void(int)>& cb);
    // void setMessageCallback(const std::function<void(MyConnection*, int)>& cb,int fd);
    // void handleConnection();
    // void handleServerEvent();
    // void handleClientEvent(int c_sockfd);
    // void newConnection();

private:
    MyEventLoop *event_loop_;
    // MySocket* serv_sock_;
    // MyChannel* serv_channel_;
    // std::function<void(int)> connection_callback_;
    std::function<void(int)> message_callback_;

};
