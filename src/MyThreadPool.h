#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "MyEventLoop.h"

class MyThreadPool {
private:
    // MyThreadPool* thread_pool_; 
    std::vector<std::thread> threads_;
    std::vector<MyEventLoop*> loops_;  // 每个线程里的 EventLoop
    std::mutex mtx_;
    bool stop_;
    int next_; // 轮询分配下标

public:
    MyThreadPool(int size = 4);
    ~MyThreadPool();

    void start();   // 创建线程并启动事件循环
    MyEventLoop* getNextLoop(); // 获取一个 sub Reactor
};
