/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_LINK_BASE_H_
#define EVAL_TUNA_LINK_BASE_H_

using std::cerr;
using std::string;
using std::queue;
using std::vector;

namespace eval { namespace tuna {

class Link_base {

 public:

  Link_base();

  void init();

  string dsn_;

  string makePGet(const vector<object_id> &ids, Tuna *T);

 protected:

/* DO I NEED THIS??

  void renewConnection() {
    Guard g(lock_);
    endWork();
    endConnection();

    startConnection();
    startWork();
  }

  void renewWork() {
    Guard g(lock_);
    endWork();
    startWork();
  }


  void startConnection() {
    conn_ = shared_ptr<connection>(new connection(dsn_));
  }

  void endConnection() {
    conn_.reset();
  }

  void startWork() {
    work_ = shared_ptr<work>(new work(*conn_));
  }

  void endWork() {
    work_->commit(); 
    work_.reset(); // this is shared_ptr function
  }
*/

  shared_ptr<connection> conn_; 
  shared_ptr<work> work_;

  Mutex lock_;

};

}} // eval::tuna 


#endif
