#include "MyAcceptor.h"

MyAcceptor::MyAcceptor(MyEventLoop *loop, const std::string &ip, int port) : event_loop_(loop), server_socket_()
{
    // server_sock_ = new MySocket();
    server_socket_.bindAddr(ip, port);
    server_socket_.startListen();

    // MyEpoll epoll;
    server_channel_ = std::make_unique<MyChannel>(server_socket_.getFd(), LISTEN_MODE_LT);
    // server_channel_ = new MyChannel(serv_sock_->getFd() );
    server_channel_->setChannelReadCallback([this]()
                                            { handleConnection(); });

    event_loop_->updateChannel(server_channel_);
}
MyAcceptor::~MyAcceptor() 
{
    if(server_channel_)
    {
        event_loop_->delChannel(server_channel_);
    }
}

void MyAcceptor::handleConnection()
{
    while (true)
    {
        int cfd = server_socket_.acceptConn();
        if (cfd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break; // backlog 已取空，正常退出循环
            }
            else if (errno == EINTR)
            {
                continue; // 信号中断，重试
            }
            else if (errno == EMFILE)
            {
                perror("accept error: too many open files");
                // 可以考虑关闭一个空闲连接释放 fd，这里先 break
                break;
            }
            else if (errno == ECONNABORTED)
            {
                perror("accept error: connection aborted");
                continue; // 忽略掉这个错误
            }
            else
            {
                perror("accept error: fatal");
                break;
            }
        }
        // 确保新连接是非阻塞的
        // int flags = fcntl(cfd, F_GETFL, 0);
        // fcntl(cfd, F_SETFL, flags | O_NONBLOCK);

        if (acceptor_callback_)
        {
            acceptor_callback_(cfd); // 交给 Server 管理
        }
    }
}

using AcceptorCallback = std::function<void(MySocket)>;
void MyAcceptor::setAcceptorCallback(AcceptorCallback cb)
{
    acceptor_callback_ = std::move(cb);
}
