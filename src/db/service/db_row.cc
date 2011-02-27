/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

#ifndef EVAL_TUNA_DB_ROW__HPP_
#define EVAL_TUNA_DB_ROW__HPP_

namespace eval { namespace tuna {

DbRow::DbRow(object_id id) {
  id_ = id; 
  flag(TUNA_EMPTY);
}

void DbRow::flag(int flag) {
  flag_ = flag;
}

void DbRow::callBack(shared_ptr<QueueLink> ln, query_id qid) {
  ln_ = ln;
  qid_ = qid;
}

void DbRow::resolvePending() {
  if (flag_ == TUNA_RESERVED) {
    ln_->resolveToQuery(qid_);
  }
}

result DbRow::getResult() {
  if (flag_ == TUNA_NON_EXISTENT) {
    throw("red ne postoji.");
  }

  if (flag_ == TUNA_OK || flag_ == TUNA_MODIFIED) {
    return record_;
  }
}

}} // eval::tuna 


#endif
