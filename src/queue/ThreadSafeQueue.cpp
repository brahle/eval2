#include <queue>

#include <boost/scoped_ptr.hpp>
#include <thrift/concurrency/Mutex.h>

#include "queue/ThreadSafeQueue.h"

using boost::scoped_ptr;

using namespace apache::thrift::concurrency;

namespace eval {

namespace queue {

template<class T>
void SyncedQueue<T>::push(const T &member) {
  Guard(this->lock_);
  queue_->push(member);
}

template<class T>
T& SyncedQueue<T>::pop() {
  Guard(this->lock_);
  
  return queue_->pop();
}

template<class T>
size_t SyncedQueue<T>::size() {
  Guard(this->lock_);

  return queue_->size();
}

template<class T>
bool SyncedQueue<T>::empty() {
  Guard(this->lock_);
  
  return queue_->empty();
};


} // namespace

} // namespace
