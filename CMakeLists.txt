cmake_minimum_required(VERSION 3.10)
project(HttpServer LANGUAGES CXX)

# 使用 clang++ 作为编译器
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O2 -DNDEBUG")

# 源文件
set(SOURCES
    src/MyEpoll.cpp
    src/MySocket.cpp
    src/MyChannel.cpp
    src/MyEventLoop.cpp
    src/MyServer.cpp
    src/MyAcceptor.cpp
    src/MyConnection.cpp
    src/MyThreadPool.cpp
)

# 可执行文件
add_executable(client client.cpp ${SOURCES})
add_executable(server server.cpp ${SOURCES})
# add_executable(test   test.cpp   ${SOURCES})
