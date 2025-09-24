#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/uio.h>
#include <cstring>
#include <algorithm> 

class MyBuffer
{

public:
    MyBuffer();
    ~MyBuffer();
    void readToBuffer(const int &fd);
    ssize_t writeFromBuffer(const int &fd);
    void addToBuffer(const char *data, size_t len);
    void addToBuffer(const std::string &data);
    void cleanBuffer();
    size_t getUnreadSize() const { return cur_write_index_ - cur_read_index_; }
    std::string getContentAsString();
    std::string getContentAsString(size_t len);

private:
    std::vector<char> buffer_;
    size_t cur_read_index_;
    size_t cur_write_index_;
    // size_t writable_size_;
};
