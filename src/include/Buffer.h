#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <sys/uio.h>
#include <cstring>

// A simple growable byte buffer with read/write indices.
class Buffer {
public:
    explicit Buffer(size_t initialSize = 4096)
        : buffer_(initialSize), readIndex_(0), writeIndex_(0) {}

    size_t readableBytes() const { return writeIndex_ - readIndex_; }
    size_t writableBytes() const { return buffer_.size() - writeIndex_; }

    const char* peek() const { return buffer_.data() + readIndex_; }
    char* beginWrite() { return buffer_.data() + writeIndex_; }
    const char* beginWrite() const { return buffer_.data() + writeIndex_; }

    void retrieve(size_t len) {
        if (len >= readableBytes()) {
            retrieveAll();
        } else {
            readIndex_ += len;
        }
    }

    void retrieveAll() {
        readIndex_ = 0;
        writeIndex_ = 0;
    }

    std::string retrieveAllAsString() {
        std::string s(peek(), readableBytes());
        retrieveAll();
        return s;
    }

    void append(const char* data, size_t len) {
        ensureWritable(len);
        std::memcpy(beginWrite(), data, len);
        hasWritten(len);
    }

    void append(const std::string& s) { append(s.data(), s.size()); }

    void ensureWritable(size_t len) {
        if (writableBytes() < len) {
            makeSpace(len);
        }
    }

    void hasWritten(size_t len) { writeIndex_ += len; }

    // Read as much as possible from non-blocking fd into buffer. Returns bytes read, 0 for EOF, -1 on error.
    ssize_t readFd(int fd, int* savedErrno) {
        // Use readv to minimize copies: first iov targets remaining space, second a large stack buffer.
        char extrabuf[65536];
        struct iovec vec[2];
        const size_t writable = writableBytes();
        vec[0].iov_base = beginWrite();
        vec[0].iov_len = writable;
        vec[1].iov_base = extrabuf;
        vec[1].iov_len = sizeof(extrabuf);

        const int iovcnt = (writable < sizeof(extrabuf)) ? 2 : 1;
        ssize_t n = ::readv(fd, vec, iovcnt);
        if (n < 0) {
            if (savedErrno) *savedErrno = errno;
            return n;
        } else if (n <= static_cast<ssize_t>(writable)) {
            hasWritten(static_cast<size_t>(n));
        } else {
            hasWritten(writable);
            append(extrabuf, static_cast<size_t>(n - writable));
        }
        return n;
    }

    // Write as much as possible to fd from buffer. Returns bytes written, -1 on error.
    ssize_t writeFd(int fd, int* savedErrno) {
        size_t nToWrite = readableBytes();
        ssize_t n = ::write(fd, peek(), nToWrite);
        if (n < 0) {
            if (savedErrno) *savedErrno = errno;
            return n;
        }
        retrieve(static_cast<size_t>(n));
        return n;
    }

private:
    void makeSpace(size_t len) {
        // If reclaiming front space is enough, move readable data to front.
        if (writableBytes() + readIndex_ >= len) {
            size_t readable = readableBytes();
            std::memmove(buffer_.data(), buffer_.data() + readIndex_, readable);
            readIndex_ = 0;
            writeIndex_ = readable;
        } else {
            // Expand buffer
            buffer_.resize(writeIndex_ + len);
        }
    }

    std::vector<char> buffer_;
    size_t readIndex_;
    size_t writeIndex_;
};
