/**
 * Copyright 2011 Matija Osrecki
 *
 * This file is part of Evaluator.
 * 
 * Evaluator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * Evaluator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public
 * License along with Evaluator. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef EVAL_QUEUE_SYNCED_QUEUE_H_
#define EVAL_QUEUE_SYNCED_QUEUE_H_

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

#endif  // EVAL_QUEUE_SYNCED_QUEUE_H_
