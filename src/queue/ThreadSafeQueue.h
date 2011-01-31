#ifndef EVAL_QUEUE_SYNCED_QUEUE_H_
#define EVAL_QUEUE_SYNCED_QUEUE_H_

namespace eval {

namespace queue {

template <class T> class SyncedQueueIf {

 public:
 
  virtual void push(const T &member) = 0;
  
  virtual T pop() = 0;
  
  virtual size_t size() = 0;
  
  virtual bool empty() = 0;
      
};

} // namespace

} // namespace


#endif  // EVAL_QUEUE_SYNCED_QUEUE_H_
