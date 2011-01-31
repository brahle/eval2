#ifndef EVAL_QUEUE_SYNCED_QUEUE_H_
#define EVAL_QUEUE_SYNCED_QUEUE_H_

#include <queue>

#include <boost/scoped_ptr.hpp>
#include <thrift/concurrency/Mutex.h>

namespace eval {

namespace queue {

template <class T> class SyncedQueue {

 public:
 
  SyncedQueue() {} 
 
  virtual void push(T &member);
  
  T* pop();
  
  size_t size();
  
  bool empty();
      
 
 private:
 
  std::queue<T> queue_;
  
  apache::thrift::concurrency::Mutex lock_;

};

} // namespace

} // namespace


#endif  // EVAL_QUEUE_SYNCED_QUEUE_H_
