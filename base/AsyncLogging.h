// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#pragma once

#include "BlockingQueue.h"
#include "BoundedBlockingQueue.h"
#include "CountDownLatch.h"
#include "LogStream.h"

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

namespace muduo {

class AsyncLogging : noncopyable {
   public:
    AsyncLogging(const string& basename, off_t rollSize, int flushInterval = 3);

    ~AsyncLogging() {
        if (running_) {
            stop();
        }
    }

    void append(const char* logline, int len);

    void start() {
        // running_ = true;
        // thread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond_.notify_one();
        thread_.join();
    }

   private:
    void threadFunc();

    typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    const string basename_;
    const off_t rollSize_;
    std::thread thread_;
    muduo::CountDownLatch latch_;
    std::mutex mutex_;
    std::condition_variable cond_;
    BufferPtr currentBuffer_;
    BufferPtr nextBuffer_;
    BufferVector buffers_;
};

}  // namespace muduo
