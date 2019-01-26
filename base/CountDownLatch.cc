// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#include "CountDownLatch.h"

using namespace muduo;

CountDownLatch::CountDownLatch(int count)
    : mutex_(),
      // condition_(mutex_),
      count_(count) {}

void CountDownLatch::wait() {
    // MutexLockGuard lock(mutex_);
    std::unique_lock<std::mutex> lock(mutex_);
    while (count_ > 0) {
        condition_.wait(lock);
    }
}

void CountDownLatch::countDown() {
    // MutexLockGuard lock(mutex_);
    std::lock_guard<std::mutex> lock(mutex_);
    --count_;
    if (count_ == 0) {
        condition_.notify_all();
    }
}

int CountDownLatch::getCount() const {
    // MutexLockGuard lock(mutex_);
    std::lock_guard<std::mutex> lock(mutex_);
    return count_;
}
