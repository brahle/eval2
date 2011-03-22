/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_LINK_BASE_H_
#define EVAL_TUNA_LINK_BASE_H_

namespace eval { namespace tuna {

class Link_base {

 public:

  Link_base();

  void init();

  void resetConnection();

  string dsn_;

 protected:


  shared_ptr<connection> conn_; 
  shared_ptr<work> work_;

  Mutex lock_;

};

}} // eval::tuna 


#endif
