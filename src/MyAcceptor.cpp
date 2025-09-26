#include "MyAcceptor.h"

MyAcceptor::MyAcceptor(MyEventLoop *loop, const std::string &ip, int port) : event_loop_(loop)
{
    serv_sock_ = new MySocket();
    serv_sock_->bindAddr(ip, port);
    serv_sock_->startListen();

    // MyEpoll epoll;
    serv_channel_ = new MyChannel(serv_sock_->getFd(), ACCEPT_MODE);
    // serv_channel_ = new MyChannel(serv_sock_->getFd() );
    serv_channel_->setChannelCallback([this]()
                               { handleConnection(); });

    event_loop_->updateChannel(serv_channel_);
}
MyAcceptor::~MyAcceptor()
{
    delete serv_channel_;
    delete serv_sock_;
}

void MyAcceptor::handleConnection()
{
    while (true)
    {
        int cfd = serv_sock_->acceptConn();
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
