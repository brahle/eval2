#include <queue>

#include <boost/scoped_ptr.hpp>
#include <thrift/concurrency/Mutex.h>

#include "queue/ThreadSafeQueue.h"

using boost::scoped_ptr;

using namespace apache::thrift::concurrency;

namespace eval {

namespace queue {

template<class T> class SyncedQueue : SyncedQueueIf<T> {
 public:
   
  SyncedQueue() {
    queue_ = scoped_ptr< std::queue<T> >(new std::queue<T>());
  }
   
  void push(const T &member) {
    Guard(this->lock_);
    queue_->push(member);
  }
  
  T& pop() {
    Guard(this->lock_);
    
    return queue_->pop();
  }
  
  size_t size() {
    Guard(this->lock_);

    return queue_->size();
  }
  
  bool empty() {
    Guard(this->lock_);
    
    return queue_->empty();
  };
 
 private:
 
  scoped_ptr< std::queue<T> > queue_;
  
  Mutex lock_;
  
};

} // namespace

} // namespace
