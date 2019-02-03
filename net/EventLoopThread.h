// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao
//
// This is a public header file, it must only include public header files.

#pragma once

#include "../base/noncopyable.h"

#include <condition_variable>
#include <mutex>
#include <thread>

namespace muduo {
namespace net {

class EventLoop;

class EventLoopThread : noncopyable {
   public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
    ~EventLoopThread();
    EventLoop* startLoop();

   private:
    void threadFunc();

    EventLoop* loop_;
    bool exiting_;
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    ThreadInitCallback callback_;
};

}  // namespace net
}  // namespace muduo
