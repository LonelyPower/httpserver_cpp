#include "MyEventLoop.h"
#include "MyChannel.h"
// #include "MyEpoll.h"
// #include <vector>
#include <errno.h>
#include "MyConnection.h"
#include "config.h"
MyConnection::MyConnection(MyEventLoop *loop, MySocket &&client_socket) : event_loop_(loop), inputBuffer_(), outputBuffer_(), connection_callback_(nullptr)
{
    if (event_loop_)
    {
        client_socket_ = std::move(client_socket);
        client_channel_ = std::make_unique<MyChannel>(client_socket_.getFd(), LISTEN_MODE_ET);

        client_channel_->setChannelReadCallback([this]()
                                         {
            // cout<<"3 Read callback invoked for connection fd: " << channel_->getFd() << endl;
        // Loop read for EPOLLET until EAGAIN
        this->inputBuffer_.readToBuffer(client_channel_->getFd());
        // cout<<"3 Finished reading data for connection fd: " << channel_->getFd() << endl;
        if (this->connection_callback_)
        {
            // cout << "4 Invoking connection callback for connection fd: " << channel_->getFd() << endl;
            this->connection_callback_(client_channel_->getFd());
            // cout << "4 Finished connection callback for connection fd: " << channel_->getFd() << endl;
        } });

        event_loop_->updateChannel(client_channel_);
        // printf("new client fd %d connected!\n", sockfd);
    }
    else
    {
        client_channel_ = nullptr;
        // printf("event_loop_ is nullptr!\n");
    }
}
MyConnection::~MyConnection()
{
    if (client_channel_)
    {
        event_loop_->delChannel(client_channel_);
        delete client_channel_;
        client_channel_ = nullptr;
    }
}

void MyConnection::setConnectionCallback(std::function<void(int)> cb)
{
    connection_callback_ = std::move(cb);
}
