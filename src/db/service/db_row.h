/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_DB_ROW_H_
#define EVAL_TUNA_DB_ROW_H_

namespace eval { namespace tuna {

class DbRow {

 public:

  DbRow(object_id id);

  void flag(int flag);

  void callBack(QueueLink *ln, query_id qid);

  void resolvePending(Tuna *T);

  int id_, flag_;

  result::tuple tup_;

  shared_ptr<result::tuple> tuple_;
  
  string echo();

 private:
 
  void *object;

  QueueLink *ln_; // which link will bring my result?
  
  query_id qid_; // and with what query_id?

};

}} // eval::tuna 


#endif
