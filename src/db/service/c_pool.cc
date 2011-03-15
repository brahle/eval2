/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

/*
  No-size constructor
  @param dsn
    database source name string
 */
ConnectionPool::ConnectionPool() {
  registerLinks(1);  
}

/*
  foreach link, trylock Queue, get his Size
  minimum size iz "free" connection
  this is maybe for private..
 */
shared_ptr<QueueLink> ConnectionPool::getFreeQueueLink() {
  Guard g(lock_);
  
  /* za sada neka stoji ovako dok jos lovimo druge greske */
  return queuePool_[0];

  unsigned int minQueueSize = 0x3f3f3f3f;

  shared_ptr<QueueLink> freeConnection;

  for (unsigned int i = 0; i < queuePool_.size(); ++i) {

    unsigned int queueSize = queuePool_[i]->curentQueueSize();

    if (queueSize < minQueueSize) {
      minQueueSize = queueSize;
      freeConnection = queuePool_[i]; 
    }

    if (!minQueueSize) return freeConnection;

  }

  return freeConnection;
}

shared_ptr<WorkLink> ConnectionPool::getFreeWorkLink() {
  Guard g(lock_);
  
  /* za sada neka stoji ovako dok jos lovimo druge greske */
  return workPool_[0];
}

void ConnectionPool::registerLinks(unsigned int size) {
  Guard g(lock_);
  
  size = 1;
  for (unsigned int i = 0; i < size; ++i) {
    queuePool_.push_back(shared_ptr<QueueLink>(  
      new QueueLink));
    
    workPool_.push_back(shared_ptr<WorkLink>(  
      new WorkLink));
  }
}


}} // eval::tuna 

