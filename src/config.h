#pragma once

#include <string>
#include <cstdint>
#include <sys/epoll.h>
constexpr int POOL_SIZE = 3;
constexpr int SERVER_PORT = 8888;
constexpr const char* SERVER_IP = "127.0.0.1";
constexpr int32_t ACCEPT_MODE = EPOLLIN;
constexpr int32_t HANDLE_MODE = EPOLLIN | EPOLLET;
constexpr int32_t MAX_EVENTS = 1024;
// constexpr bool ENABLE_LOGGING = true;
