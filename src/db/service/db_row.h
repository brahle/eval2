/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_DB_ROW__HPP_
#define EVAL_TUNA_DB_ROW__HPP_

namespace eval { namespace tuna {

class DbRow {

 public:

  DbRow(object_id id);

  void flag(int flag);

  void callBack(shared_ptr<QueueLink> ln, query_id qid);

  void resolvePending();

  result getResult();

  int id_, flag_;

 private:

  result record_;
 
  shared_ptr<QueueLink> ln_; // which link will bring my result?
  
  query_id qid_; // and with what query_id?

};

}} // eval::tuna 


#endif
