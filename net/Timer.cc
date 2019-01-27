// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#include "Timer.h"

using namespace muduo;
using namespace muduo::net;

std::atomic<std::int64_t> Timer::s_numCreated_;

void Timer::restart(Timestamp now) {
    if (repeat_) {
        // expiration_ = addTime(now, interval_);
        expiration_ = now + std::chrono::duration<double, std::chrono::seconds>(interval_);
    } else {
        expiration_ = Timestamp();
    }
}
