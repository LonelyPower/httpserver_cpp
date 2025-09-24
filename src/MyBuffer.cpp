#include "MyBuffer.h"
#include "config.h"

#include <unistd.h>
#include <errno.h>
#include <cstdio>
// #include <unistd.h>   // close
// #include <cstdio>    // perror
// #include <cstdlib>
// #include <fcntl.h>

MyBuffer::MyBuffer()
    : buffer_(BUFFER_SIZE), cur_read_index_(0), cur_write_index_(0) {}

MyBuffer::~MyBuffer()
{
    // 默认析构函数足够了，std::vector 会自动清理内存
}

void MyBuffer::readToBuffer(const int &fd)
{
    char temp_buffer[TEMP_BUFFER_SIZE];
    while (true)
    {

        ssize_t n = read(fd, temp_buffer, TEMP_BUFFER_SIZE);
        if (n > 0)
        {
            if (cur_write_index_ + n > buffer_.size())
            {
                buffer_.resize(cur_write_index_ + n + TEMP_BUFFER_SIZE); // 扩容
            }

            std::copy(temp_buffer, temp_buffer + n, buffer_.begin() + cur_write_index_);
            cur_write_index_ += n;
        }
        else if (n == 0)
        {
            // 对端关闭连接
            printf("peer closed connection\n");
            break;
        }
        else
        {
            if (errno == EINTR)
            {
                continue; // 被信号打断，重试
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            {

                break; // 非阻塞：当前无数据，不算错误
            }
            else
            {
                perror("read error");
                break;
            }
        }
    }
}

ssize_t MyBuffer::writeFromBuffer(const int &fd)  // 改为返回 ssize_t
{
    ssize_t total_written = 0;
    
    while (cur_read_index_ < cur_write_index_)
    {
        ssize_t n = write(fd, buffer_.data() + cur_read_index_, cur_write_index_ - cur_read_index_);
        if (n > 0)
        {
            cur_read_index_ += n;
            total_written += n;
        }
        else if (n == 0)
        {
            printf("connection closed during write\n");
            break;
        }
        else
        {
            if (errno == EINTR)
            {
                continue;
            }
            else if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break; // 部分写入是正常的
            }
            else
            {
                perror("write error");
                return -1;  // 返回错误
            }
        }
    }

    if (cur_read_index_ == cur_write_index_)
    {
        cleanBuffer();
    }
    
    return total_written;
}

std::string MyBuffer::getContentAsString()
{
    std::string s(buffer_.data() + cur_read_index_, getUnreadSize());
    cleanBuffer();
    // cur_read_index_ = 0;
    // cur_write_index_ = 0;
    return s;
}
std::string MyBuffer::getContentAsString(size_t len)
{
    if (len > getUnreadSize())
    {
        len = getUnreadSize();
    }
    std::string s(buffer_.data() + cur_read_index_, len);
    cur_read_index_ += len;
    if (cur_read_index_ == cur_write_index_)
    {
        cleanBuffer();
    }
    return s;
}
void MyBuffer::addToBuffer(const char *data, size_t len)
{
    if (cur_write_index_ + len > buffer_.size())
    {
        buffer_.resize(cur_write_index_ + len + TEMP_BUFFER_SIZE);
    }
    std::copy(data, data + len, buffer_.begin() + cur_write_index_);
    cur_write_index_ += len;
}
void MyBuffer::addToBuffer(const std::string &data)
{
    addToBuffer(data.data(), data.size());
}

void MyBuffer::cleanBuffer()
{
    cur_read_index_ = 0;
    cur_write_index_ = 0;
}