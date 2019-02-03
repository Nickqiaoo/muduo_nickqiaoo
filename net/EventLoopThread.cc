// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#include "EventLoopThread.h"

#include "EventLoop.h"

using namespace muduo;
using namespace muduo::net;

EventLoopThread::EventLoopThread(const ThreadInitCallback& cb)
    : loop_(NULL),
      exiting_(false),
      // thread_(std::bind(&EventLoopThread::threadFunc, this)),
      mutex_(),
      // cond_(mutex_),
      callback_(cb) {}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != NULL)  // not 100% race-free, eg. threadFunc could be running callback_.
    {
        // still a tiny chance to call destructed object, if threadFunc exits just now.
        // but when EventLoopThread destructs, usually programming is exiting anyway.
        loop_->quit();
        thread_.join();
    }
}

EventLoop* EventLoopThread::startLoop() {
    // assert(!thread_.started());
    // thread_.start();

    EventLoop* loop = NULL;
    thread_ = std::thread(std::bind(&EventLoopThread::threadFunc, this));
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (loop_ == NULL) {
            cond_.wait(lock);
        }
        loop = loop_;
    }

    return loop;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;

    if (callback_) {
        callback_(&loop);
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        loop_ = &loop;
        cond_.notify_one();
    }

    loop.loop();
    // assert(exiting_);
    std::lock_guard<std::mutex> lock(mutex_);
    loop_ = NULL;
}
