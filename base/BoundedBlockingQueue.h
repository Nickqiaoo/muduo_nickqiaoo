// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#pragma once

#include <assert.h>
#include <boost/circular_buffer.hpp>
#include <condition_variable>

namespace muduo {

template <typename T>
class BoundedBlockingQueue : noncopyable {
   public:
    explicit BoundedBlockingQueue(int maxSize)
        : queue_(maxSize) {}

    void put(const T& x) {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.full()) {
            notFull_.wait(lock);
        }
        assert(!queue_.full());
        queue_.push_back(x);
        notEmpty_.notify_one();
    }

    T take() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (queue_.empty()) {
            notEmpty_.wait(lock);
        }
        assert(!queue_.empty());
        T front(queue_.front());
        queue_.pop_front();
        notFull_.notify_one();
        return front;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    bool full() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.full();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    size_t capacity() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.capacity();
    }

   private:
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_ ;
    std::condition_variable notFull_ ;
    boost::circular_buffer<T> queue_ ;
};

}  // namespace muduo
