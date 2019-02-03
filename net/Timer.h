// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <atomic>
#include <memory>

#include "../base/Timestamp.h"
#include "../base/noncopyable.h"
#include "Callbacks.h"

namespace muduo {
namespace net {

///
/// Internal class for timer event.
///
class Timer : noncopyable {
   public:
    Timer(TimerCallback cb, Timestamp when, double interval)
        : callback_(std::move(cb)),
          expiration_(when),
          interval_(interval),
          repeat_(interval > 0.0),
          sequence_(s_numCreated_.fetch_add(1) + 1) {}

    void run() const { callback_(); }

    Timestamp expiration() const { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_; }

    void restart(Timestamp now);

    static int64_t numCreated() { return s_numCreated_.load(); }

   private:
    const TimerCallback callback_;
    Timestamp expiration_;
    const double interval_;
    const bool repeat_;
    const int64_t sequence_;

    static std::atomic<std::int64_t> s_numCreated_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_TIMER_H
