/**
 * Copyright 2011 Matija Osrecki
 *
 * TODO copy notice
 */

#ifndef EVAL_QUEUE_SYNCED_QUEUE_HPP_
#define EVAL_QUEUE_SYNCED_QUEUE_HPP_

#include <queue>

#include <thrift/concurrency/Mutex.h>

using apache::thrift::concurrency::Guard;
using apache::thrift::concurrency::Mutex;
using std::queue;

namespace eval {

  /**
   * Simple templated implementation of a synchronized queue.
   * Simple because the only methods implemented are:
   *   push, pop, size and empty.
   */
template <class T> class SyncedQueue {
 public:
  SyncedQueue() {}

  void push(const T &member) {
    Guard g(this->lock_);
    queue_.push(member);
  }

  T pop() throw(int) {
    Guard g(this->lock_);

    if (queue_.size() == 0)
      throw 1;

    T ret = queue_.front();
    queue_.pop();
    return ret;
  }

  size_t size() {
    Guard g(this->lock_);
    return queue_.size();
  }

  bool empty() {
    Guard g(this->lock_);
    return queue_.empty();
  };

 private:
  queue<T> queue_;

  Mutex lock_;
};

}  // namespace

#endif  // EVAL_QUEUE_SYNCED_QUEUE_HPP_
