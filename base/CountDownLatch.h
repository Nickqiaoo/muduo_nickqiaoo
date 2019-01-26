#pragma once

#include <condition_variable>
#include <mutex>

#include "noncopyable.h"

namespace muduo {

class CountDownLatch : noncopyable {
   public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;

   private:
    // mutable MutexLock mutex_;
    // Condition condition_ GUARDED_BY(mutex_);
    // int count_ GUARDED_BY(mutex_);
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    int count_;
};

}  // namespace muduo
