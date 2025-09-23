cmake_minimum_required(VERSION 3.10)
project(HttpServer LANGUAGES CXX)

# 指定编译器和标准
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

# -------------------------------
#  clang-tidy: 单独目标，输出日志
# -------------------------------
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    add_custom_target(run-clang-tidy
        COMMAND ${CLANG_TIDY_EXE}
            ${SOURCES} client.cpp server.cpp
            -- -I${CMAKE_SOURCE_DIR}/src -std=c++20
            > ${CMAKE_BINARY_DIR}/clang-tidy.log 2>&1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy on all sources (see clang-tidy.log)"
    )
endif()
