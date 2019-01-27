// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "Types.h"
#include "noncopyable.h"

#include <deque>
#include <vector>

namespace muduo
{

class ThreadPool : noncopyable
{
 public:
  typedef std::function<void ()> Task;

  //explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { maxQueueSize_ = maxSize; }
  void setThreadInitCallback(const Task& cb)
  { threadInitCallback_ = cb; }

  void start(int numThreads);
  void stop();

  //const string& name() const
  //{ return name_; }

  size_t queueSize() const;

  // Could block if maxQueueSize > 0
  void run(Task f);

 private:
  bool isFull() const;
  void runInThread();
  Task take();

  mutable std::mutex mutex_;
  std::condition_variable notEmpty_;
  std::condition_variable notFull_;
  //string name_;
  Task threadInitCallback_;
  std::vector<std::unique_ptr<std::thread>> threads_;
  std::deque<Task> queue_;
  size_t maxQueueSize_{0};
  bool running_{false};
};

}  // namespace muduo
