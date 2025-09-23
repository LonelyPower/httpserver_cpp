#pragma once

#include <string>
#include <cstdint>

constexpr int POOL_SIZE = 7;
constexpr int SERVER_PORT = 8080;
constexpr std::string SERVER_IP = "127.0.0.1";
constexpr int32_t ACCEPT_MODE = EPOLLIN;
constexpr int32_t HANDLE_MODE = EPOLLIN | EPOLLET; 

// constexpr bool ENABLE_LOGGING = true;
