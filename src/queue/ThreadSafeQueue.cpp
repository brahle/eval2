#include <queue>

#include <boost/scoped_ptr.hpp>
#include <thrift/concurrency/Mutex.h>

#include "queue/ThreadSafeQueue.h"

using boost::scoped_ptr;

using namespace apache::thrift::concurrency;

namespace eval {

namespace queue {

template<class T>
void SyncedQueue<T>::push(T &member) {
  Guard g(lock_);
  queue_.push(member);
}

template<class T>
T* SyncedQueue<T>::pop() {
  Guard g(lock_);
  
  if(this->size() == 0) 
    return NULL;

  T &ret = queue_.front();
  queue_.pop();
  return &ret;
}

template<class T>
size_t SyncedQueue<T>::size() {
  Guard g(lock_);

  return queue_.size();
}

template<class T>
bool SyncedQueue<T>::empty() {
  Guard g(lock_);
  
  return queue_.empty();
};


} // namespace

} // namespace
