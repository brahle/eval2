/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_CONNECTION_POLL_H_
#define EVAL_CONNECTION_POLL_H_

using std::string;
using std::vector;

namespace eval { namespace tuna {

class ConnectionPool {

 public:

  ConnectionPool();

  shared_ptr<QueueLink> getFreeQueueLink();

  shared_ptr<WorkLink> getFreeWorkLink();

 private:

  void registerLinks(unsigned int size);

  vector<shared_ptr<QueueLink> > queuePool_;

  vector<shared_ptr<WorkLink> > workPool_;

  Mutex lock_;

};

}} // eval::tuna 

#endif
