#ifndef EVAL_QUEUE_SYNCED_QUEUE_H_
#define EVAL_QUEUE_SYNCED_QUEUE_H_

#include <queue>

#include <boost/scoped_ptr.hpp>
#include <thrift/concurrency/Mutex.h>

namespace eval {

namespace queue {

template <class T> class SyncedQueue {

 public:
 
  SyncedQueue() { 
//    this->queue_ = boost::scoped_ptr< std::queue<T> >(new std::queue<T>());
  } 
 
  void push(const T &member);
  
  T& pop();
  
  virtual size_t size();
  
  virtual bool empty();
      
 
 private:
 
  boost::scoped_ptr< std::queue<T> > queue_;
  
  apache::thrift::concurrency::Mutex lock_;

};

} // namespace

} // namespace


#endif  // EVAL_QUEUE_SYNCED_QUEUE_H_
