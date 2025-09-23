#include "MyThreadPool.h"

#include <iostream>

MyThreadPool::MyThreadPool(int size) : stop_(false), next_(0)
{
    loops_.resize(size, nullptr);
}

MyThreadPool::~MyThreadPool()
{
    stop_ = true;
    for (auto &t : threads_)
    {
        if (t.joinable())
            t.join();
    }
    for (auto &loop : loops_)
    {
        delete loop;
    }
    threads_.clear();
}

void MyThreadPool::start()
{
    for (size_t i = 0; i < loops_.size(); ++i)
    {
        threads_.emplace_back([this, i]()
        {
            // 每个线程里创建一个 EventLoop
            MyEventLoop *loop = new MyEventLoop();
            {
                std::lock_guard<std::mutex> lock(mtx_);
                loops_[i] = loop;
            }
            std::cout << "Sub Reactor " << i << " started" << std::endl;
            loop->startLoop(); // 线程阻塞在事件循环
        });
    }
}

MyEventLoop *MyThreadPool::getNextLoop()
{
    std::lock_guard<std::mutex> lock(mtx_);
    if (loops_.empty())
    {
        return nullptr;
    }
    MyEventLoop *loop = loops_[next_];
    next_ = (next_ + 1) % loops_.size(); // round-robin 轮询
    return loop;
}
