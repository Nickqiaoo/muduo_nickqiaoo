// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#pragma once

#include <assert.h>
#include <condition_variable>
#include <deque>
#include <mutex>

#include "noncopyable.h"

namespace muduo {

template <typename T>
class BlockingQueue : noncopyable {
   public:
    BlockingQueue()
        : mutex_(),
          // notEmpty_(mutex_),
          queue_() {}

    void put(const T& x) {
        // MutexLockGuard lock(mutex_);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push_back(x);
        // notEmpty_.notify(); // wait morphing saves us
        notEmpty_.notify_one();
        // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
    }

    void put(T&& x) {
        // MutexLockGuard lock(mutex_);
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push_back(std::move(x));
        // notEmpty_.notify();
        notEmpty_.notify_one();
    }

    T take() {
        // MutexLockGuard lock(mutex_);
        std::unique_lock<std::mutex> lock(mutex_);
        // always use a while-loop, due to spurious wakeup
        while (queue_.empty()) {
            notEmpty_.wait(lock);
        }
        assert(!queue_.empty());
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return std::move(front);
    }

    size_t size() const {
        // MutexLockGuard lock(mutex_);
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

   private:
    // mutable MutexLock mutex_;
    // Condition         notEmpty_ GUARDED_BY(mutex_);
    // std::deque<T>     queue_ GUARDED_BY(mutex_);
    mutable std::mutex mutex_;
    std::condition_variable notEmpty_;
    std::deque<T> queue_;
};

}  // namespace muduo
